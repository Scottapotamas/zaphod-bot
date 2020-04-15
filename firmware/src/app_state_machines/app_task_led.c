/* ----- System Includes ---------------------------------------------------- */

#include <event_subscribe.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_events.h"
#include "app_signals.h"
#include "app_times.h"
#include "qassert.h"

#include "app_task_led.h"
#include "led_interpolator.h"

#include "configuration.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskLedConstructor( AppTaskLed *me );

PRIVATE void  AppTaskLed_initial( AppTaskLed *me, const StateEvent *e );
PRIVATE STATE AppTaskLed_main( AppTaskLed *me, const StateEvent *e );
PRIVATE STATE AppTaskLed_inactive( AppTaskLed *me, const StateEvent *e );
PRIVATE STATE AppTaskLed_active( AppTaskLed *me, const StateEvent *e );
PRIVATE STATE AppTaskLed_active_manual( AppTaskLed *me, const StateEvent *e );

PRIVATE void AppTaskLed_clear_queue( AppTaskLed *me );
PRIVATE void AppTaskLed_add_event_to_queue( AppTaskLed *me, const StateEvent *e );
PRIVATE void AppTaskLed_commit_queued_fade( AppTaskLed *me );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskLedCreate( AppTaskLed *  me,
                  StateEvent *  eventQueueData[],
                  const uint8_t eventQueueSize,
                  StateEvent *  lightingQueue[],
                  const uint8_t lightingQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof( AppTaskLed ) );

    // Initialise State Machine State
    AppTaskLedConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask *)me,
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
PRIVATE void AppTaskLed_initial( AppTaskLed *me, const StateEvent *e __attribute__( ( __unused__ ) ) )
{
    eventSubscribe( (StateTask *)me, LED_OFF );
    eventSubscribe( (StateTask *)me, LED_QUEUE_ADD );
    eventSubscribe( (StateTask *)me, LED_CLEAR_QUEUE );
    eventSubscribe( (StateTask *)me, LED_QUEUE_START );
    eventSubscribe( (StateTask *)me, LED_QUEUE_START_SYNC );

    eventSubscribe( (StateTask *)me, LED_ALLOW_MANUAL_CONTROL );
    eventSubscribe( (StateTask *)me, LED_RESTRICT_MANUAL_CONTROL );
    eventSubscribe( (StateTask *)me, LED_MANUAL_SET );

    eventSubscribe( (StateTask *)me, ANIMATION_COMPLETE );
    eventSubscribe( (StateTask *)me, PATHING_STARTED );
    eventSubscribe( (StateTask *)me, PATHING_COMPLETE );

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

            return 0;

        case LED_QUEUE_ADD:
            AppTaskLed_add_event_to_queue( me, e );
            return 0;

        case LED_CLEAR_QUEUE:
            AppTaskLed_clear_queue( me );
            return 0;

        case LED_QUEUE_START:
            // 0 ID's are run immediately
            me->identifier_to_execute = 0;
            STATE_TRAN( AppTaskLed_active );
            return 0;

        case LED_QUEUE_START_SYNC: {
            if( eventQueueUsed( &me->super.requestQueue ) )
            {
                // peek at the next queue item
                StateEvent *next = eventQueuePeek( &me->super.requestQueue );
                ASSERT( next );
                LightingPlannerEvent *lpe          = (LightingPlannerEvent *)next;
                Fade_t *              pending_fade = &lpe->animation;

                uint16_t id_requested     = ( (BarrierSyncEvent *)e )->id;    // ID coming in from the barrier event
                me->identifier_to_execute = id_requested;

                // Handle when lighting event queue ID is behind the requested ID
                while( id_requested > pending_fade->identifier
                       && eventQueueUsed( &me->super.requestQueue ) )
                {
                    config_report_error( "Culling Fade" );

                    // Delete the current peeked fade
                    next = eventQueueGet( &me->super.requestQueue );
                    eventPoolGarbageCollect( (StateEvent *)next );

                    // Peek the next event in the queue
                    next = eventQueuePeek( &me->super.requestQueue );
                    ASSERT( next );
                    lpe          = (LightingPlannerEvent *)next;
                    pending_fade = &lpe->animation;
                }

                // Lighting event queue ID matches the sync ID, so execute the lighting animation
                STATE_TRAN( AppTaskLed_active );
            }
            else    // no events in the queue
            {
                config_report_error( "LED Sync Fail - no events" );
            }

            return 0;
        }

        case LED_ALLOW_MANUAL_CONTROL:
            STATE_TRAN( AppTaskLed_active_manual );
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
            // todo tell the ui about the led state in use

            AppTaskLed_commit_queued_fade( me );
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
            return 0;

        case STATE_STEP1_SIGNAL:
            AppTaskLed_commit_queued_fade( me );
            AppTaskLed_commit_queued_fade( me );
            return 0;

        case ANIMATION_COMPLETE: {
            // the led interpolation engine has completed the animation execution,
            // loop around to process another event with the same ID, or go back to inactive and wait for sync
            if( eventQueueUsed( &me->super.requestQueue ) )
            {
                StateEvent *next = eventQueuePeek( &me->super.requestQueue );
                ASSERT( next );
                LightingPlannerEvent *lpe            = (LightingPlannerEvent *)next;
                Fade_t *              next_animation = &lpe->animation;

                if( next_animation->identifier >= me->identifier_to_execute )
                {
                    stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
                }
                else
                {
                    // todo reassess this state transition conditional
                    STATE_TRAN( AppTaskLed_inactive );
                }
            }
            else if( led_interpolator_is_empty() )
            {
                STATE_TRAN( AppTaskLed_inactive );
            }

            return 0;
        }

        case PATHING_STARTED:
            me->identifier_to_execute = ( (BarrierSyncEvent *)e )->id;
            led_interpolator_start_id( ( (BarrierSyncEvent *)e )->id );    // start queued LED animation
            return 0;

        case PATHING_COMPLETE:
            // todo use this 'move completed' along with ANIMATION_COMPLETE to work out:
            //      if the lighting is over-running the movement -> off -> inactive
            //      if the movement has stopped and no LED queue items are -> inactive

            return 0;

        case LED_QUEUE_ADD:
            AppTaskLed_add_event_to_queue( me, e );
            return 0;

        case LED_CLEAR_QUEUE:
            AppTaskLed_clear_queue( me );
            STATE_TRAN( AppTaskLed_inactive );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            led_interpolator_stop();
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskLed_active_manual( AppTaskLed *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
            led_interpolator_manual_override_on();
            return 0;

        case LED_RESTRICT_MANUAL_CONTROL:
            STATE_TRAN( AppTaskLed_inactive );
            return 0;

        case LED_MANUAL_SET: {
            LightingManualEvent *lme = (LightingManualEvent *)e;

            if( lme )
            {
                led_interpolator_manual_control_set( lme->colour.hue,
                                                     lme->colour.saturation,
                                                     lme->colour.intensity,
                                                     lme->enabled );
            }
        }
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            led_interpolator_manual_override_release();
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
AppTaskLed_clear_queue( AppTaskLed *me )
{
    // Empty the queue
    StateEvent *next = eventQueueGet( &me->super.requestQueue );
    while( next )
    {
        eventPoolGarbageCollect( (StateEvent *)next );
        next = eventQueueGet( &me->super.requestQueue );
    }

    //update UI with queue content count
    config_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void AppTaskLed_add_event_to_queue( AppTaskLed *me, const StateEvent *e )
{
    LightingPlannerEvent *lpe = (LightingPlannerEvent *)e;

    // Add the LED animation request to the queue if we have room
    uint8_t queue_usage = eventQueueUsed( &me->super.requestQueue );
    if( queue_usage <= LED_QUEUE_DEPTH_MAX )
    {
        if( lpe->animation.duration )
        {
            eventQueuePutFIFO( &me->super.requestQueue, (StateEvent *)e );
        }
    }
    else
    {
        //queue full, clearly the input processor isn't abiding by the spec.
        config_report_error( "LED Queue Full" );
    }

    config_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

PRIVATE void AppTaskLed_commit_queued_fade( AppTaskLed *me )
{
    if( led_interpolator_is_ready_for_next()
        && eventQueueUsed( &me->super.requestQueue ) )
    {
        StateEvent *next = eventQueueGet( &me->super.requestQueue );
        ASSERT( next );

        LightingPlannerEvent *lpe            = (LightingPlannerEvent *)next;
        Fade_t *              next_animation = &lpe->animation;

        if( next_animation->duration )
        {
            // Add the valid lighting 'fade' animation to the ping-pong buffer
            led_interpolator_set_objective( next_animation );
            eventPoolGarbageCollect( (StateEvent *)next );
        }
    }

    config_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* ----- End ---------------------------------------------------------------- */
