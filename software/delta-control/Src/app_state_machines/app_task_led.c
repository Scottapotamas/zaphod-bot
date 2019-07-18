/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <event_subscribe.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_times.h"
#include "app_signals.h"
#include "app_events.h"
#include "qassert.h"

#include "app_task_led.h"
#include "led_interpolator.h"

#include "configuration.h"
#include "hal_gpio.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void 	AppTaskLedConstructor( AppTaskLed *me );

PRIVATE void 	AppTaskLed_initial	( AppTaskLed *me, const StateEvent *e );
PRIVATE STATE 	AppTaskLed_main		( AppTaskLed *me, const StateEvent *e );
PRIVATE STATE 	AppTaskLed_inactive	( AppTaskLed *me, const StateEvent *e );
PRIVATE STATE 	AppTaskLed_active	( AppTaskLed *me, const StateEvent *e );



/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskLedCreate( AppTaskLed 	    *me,
                  StateEvent        *eventQueueData[ ],
                  const uint8_t     eventQueueSize,
                  StateEvent        *lightingQueue[ ],
                  const uint8_t     lightingQueueSize)
{
    // Clear all task data
    memset( me, 0, sizeof(AppTaskLed) );

    // Initialise State Machine State
    AppTaskLedConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask*)me,
                            eventQueueData,
                            eventQueueSize,
                            lightingQueue,
                            lightingQueueSize );
}

/* ----- Private Functions -------------------------------------------------- */

// State Machine Construction
PRIVATE void AppTaskLedConstructor( AppTaskLed *me )
{
    stateTaskCtor( &me->super, (State)&AppTaskLed_initial );
}

/* -------------------------------------------------------------------------- */

// State Machine Initial State
PRIVATE void AppTaskLed_initial( AppTaskLed *me, const StateEvent *e __attribute__((__unused__)) )
{
    eventSubscribe( (StateTask*)me, LED_OFF );
    eventSubscribe( (StateTask*)me, LED_QUEUE_ADD );
    eventSubscribe( (StateTask*)me, LED_CLEAR_QUEUE );
    eventSubscribe( (StateTask*)me, LED_QUEUE_START );
    eventSubscribe( (StateTask*)me, LED_QUEUE_START_SYNC );
    eventSubscribe( (StateTask*)me, LED_QUEUE_PAUSE );

    eventSubscribe( (StateTask*)me, ANIMATION_COMPLETE );

    led_interpolator_init();

    STATE_INIT( &AppTaskLed_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskLed_main( AppTaskLed *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	return 0;

        case STATE_STEP1_SIGNAL:
            STATE_TRAN( AppTaskLed_inactive );
            return 0;

    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskLed_inactive( AppTaskLed *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

            // Check for queued events
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );


        	return 0;


        case STATE_STEP1_SIGNAL:
        {
            uint8_t queue_usage = eventQueueUsed( &me->super.requestQueue );
            // Check the queue for pending movements, tell the supervisor the next value in the queue
            StateEvent *pendingAnimation = eventQueuePeek(&me->super.requestQueue);

            if (pendingAnimation)
            {
                LightingPlannerEvent *lpe = (LightingPlannerEvent *) pendingAnimation;
                uint16_t id_in_queue = ((Fade_t*)&lpe->animation)->identifier;

                // tell the supervisor what the next move in the queue in
                BarrierSyncEvent *led_sync_event = EVENT_NEW( BarrierSyncEvent, QUEUE_SYNC_LED_NEXT );
                led_sync_event->id = id_in_queue;
                eventPublish( (StateEvent*)led_sync_event );
            }

            // update the UI with the queue depth
            config_set_led_queue_depth(eventQueueUsed(&me->super.requestQueue));
        }
        return 0;

        case LED_QUEUE_ADD:
        {
            LightingPlannerEvent *lpe = (LightingPlannerEvent*)e;

            // Add the movement request to the queue if we have room
            uint8_t queue_usage = eventQueueUsed( &me->super.requestQueue );
            if( queue_usage <= LED_QUEUE_DEPTH_MAX )
            {
                if( lpe->animation.duration )
                {
                    eventQueuePutFIFO( &me->super.requestQueue, (StateEvent*)e );
                    queue_usage += 1;   // queue depth value passed to the UI should include the one we just added
                }
            }
            else
            {
                //queue full, clearly the input motion processor isn't abiding by the spec.
                //shutdown
                eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
                //todo make errors for led
            }

            config_set_led_queue_depth(eventQueueUsed(&me->super.requestQueue));
            return 0;
        }

        case LED_CLEAR_QUEUE:
        {
            // Empty the queue
            StateEvent * next = eventQueueGet( &me->super.requestQueue );
            while( next )
            {
                eventPoolGarbageCollect( (StateEvent*)next );
                next = eventQueueGet( &me->super.requestQueue );
            }

            //update UI with queue content count
            config_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );

            return 0;
        }

        case LED_QUEUE_START:
        {
            STATE_TRAN( AppTaskLed_active);
            return 0;
        }

        case LED_QUEUE_START_SYNC:
        {
            // identifier_to_execute
            uint16_t id_requested = ((BarrierSyncEvent*)e)->id;
            uint16_t id_in_queue = 0;

            // get the next event off the queue and get the ID
            StateEvent * nextFade = eventQueuePeek( &me->super.requestQueue );

            if(nextFade)
            {
                LightingPlannerEvent *lpe = (LightingPlannerEvent*)nextFade;
                Fade_t * next_fade = &lpe->animation;
                id_in_queue = next_fade->identifier;
            }

            if( id_requested >= id_in_queue)
            {
                me->identifier_to_execute = id_requested;
                STATE_TRAN( AppTaskLed_active );
            }

            return 0;
        }

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskLed_active( AppTaskLed *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            // todo tell the ui about the led state in use
            return 0;

        case STATE_STEP1_SIGNAL:
        {
            // check for pending events in the queue
            uint8_t queue_usage = eventQueueUsed(&me->super.requestQueue);

            if (queue_usage)
            {
                // grab an event off the queue
                StateEvent *next = eventQueueGet(&me->super.requestQueue);

                // start up the pathing engine
                if (next)
                {
                    LightingPlannerEvent *lpe = (LightingPlannerEvent *) next;
                    Fade_t *next_animation = &lpe->animation;

                    if (me->identifier_to_execute == 0 || me->identifier_to_execute >= next_animation->identifier)
                    {
                        led_interpolator_set_objective( next_animation );

                        // remove it from the queue
                        eventPoolGarbageCollect((StateEvent *) next);
                    }
                }
            }
            else
            {
                // no events in the queue
                STATE_TRAN(AppTaskLed_inactive);
            }

            config_set_led_queue_depth(eventQueueUsed(&me->super.requestQueue));

            return 0;
        }

        case ANIMATION_COMPLETE:
        {
            // the led interpolation engine has completed the animation execution,
            // loop around to process another event, or go back to inactive and wait for sync

            StateEvent * next = eventQueuePeek( &me->super.requestQueue );
            LightingPlannerEvent *lpe = (LightingPlannerEvent*)next;

            if(next)
            {
                if( me->identifier_to_execute == 0 || me->identifier_to_execute >= &lpe->animation.identifier )
                {
                    stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
                }
                else
                {
                    STATE_TRAN( AppTaskLed_inactive );
                }
            }
            config_set_led_queue_depth(eventQueueUsed(&me->super.requestQueue));

            return 0;
        }

        case LED_QUEUE_ADD:
        {
            //already in motion, so add this one to the queue
            LightingPlannerEvent *lpe = (LightingPlannerEvent*)e;

            // Add the movement request to the queue if we have room
            uint8_t queue_usage = eventQueueUsed( &me->super.requestQueue );
            if( queue_usage <= LED_QUEUE_DEPTH_MAX )
            {
                if( lpe->animation.duration)
                {
                    eventQueuePutFIFO( &me->super.requestQueue, (StateEvent*)e );
                    queue_usage += 1;   // queue depth value passed to the UI should include the one we just added
                }
            }
            else
            {
                //queue full, clearly the input processor isn't abiding by the spec.
                //shutdown
                uint8_t error = 1;
                uint8_t breakpoint_line = 1;

            }

            config_set_led_queue_depth(eventQueueUsed(&me->super.requestQueue));
        }
            return 0;

        case LED_CLEAR_QUEUE:
        {
            // Empty the queue
            StateEvent * next = eventQueueGet( &me->super.requestQueue );
            while( next )
            {
                eventPoolGarbageCollect( (StateEvent*)next );
                next = eventQueueGet( &me->super.requestQueue );
            }

            //update UI with queue content count
            config_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );

            STATE_TRAN( AppTaskLed_inactive );   //go back to idle
            return 0;
        }

        case LED_QUEUE_PAUSE:
            STATE_TRAN( AppTaskLed_inactive );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* ----- End ---------------------------------------------------------------- */
