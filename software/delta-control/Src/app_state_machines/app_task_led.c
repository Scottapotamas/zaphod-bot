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
    eventSubscribe( (StateTask*)me, LED_ADD_REQUEST );
    eventSubscribe( (StateTask*)me, LED_CLEAR_QUEUE );

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
            // Check the queue for pending animations
        	LightingPlannerEvent * next = (LightingPlannerEvent*)eventQueueGet( &me->super.requestQueue );

        	if( next )
            {
                /* Setup next animation */
                me->lpe = *next;
                eventPoolGarbageCollect( (StateEvent*)next );
                STATE_TRAN( AppTaskLed_active );
            }

            config_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );

            return 0;
        }

        case LED_ADD_REQUEST:
			{
				//want to do a animation, process immediately without the queue
				LightingPlannerEvent *ape = (LightingPlannerEvent*)e;
				me->lpe = *ape; /* Save event */
				STATE_TRAN( AppTaskLed_active );
			}
			return 0;


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
        {
            Fade_t * current_move = &me->lpe.animation;

            //send the current fade object pointer to the animation engine
            led_interpolator_set_objective( current_move );

            //we expect the animation to be completed within the specified duration
            //check back with the pathing engine when that duration is up
            eventTimerStartOnce( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 current_move->duration + 1 );

//            config_set_motion_state( TASKSTATE_MOTION_ACTIVE );

        }
            return 0;

        case STATE_TIMEOUT1_SIGNAL:

            if( led_interpolator_get_move_done() )
            {
                STATE_TRAN( AppTaskLed_inactive );
            }
            else
            {
                //check back in a millisecond
                eventTimerStartOnce( &me->timer1,
                                     (StateTask* )me,
                                     (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                     1 );
            }

            return 0;

        case STATE_STEP1_SIGNAL:
            if( eventTimerIsActive(&me->timer1) )
            {
                if( led_interpolator_get_move_done() )
                {
                    STATE_TRAN( AppTaskLed_inactive );
                }
            }

            return 0;

        case LED_ADD_REQUEST:
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
                    stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
                }
            }
            else
            {
                //queue full, clearly the input processor isn't abiding by the spec.
                //shutdown
                uint8_t error = 1;
                uint8_t breakpoint_line = 1;

            }
//            config_set_motion_queue_depth( queue_usage );
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
//            config_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
        }
            return 0;


        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* ----- End ---------------------------------------------------------------- */
