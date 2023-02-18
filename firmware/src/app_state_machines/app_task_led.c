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

#include "hal_systick.h"
#include "led.h"
#include "user_interface.h"

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
appTaskLedCreate( AppTaskLed   *me,
                  StateEvent   *eventQueueData[],
                  const uint8_t eventQueueSize,
                  StateEvent   *lightingQueue[],
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
    eventSubscribe( (StateTask *)me, LED_QUEUE_CLEAR );
    eventSubscribe( (StateTask *)me, LED_QUEUE_START );

    eventSubscribe( (StateTask *)me, LED_REQUEST_MANUAL_CONTROL );
    eventSubscribe( (StateTask *)me, LED_RELEASE_MANUAL_CONTROL );
    eventSubscribe( (StateTask *)me, LED_MANUAL_SET );

    eventSubscribe( (StateTask *)me, ANIMATION_COMPLETE );

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

        case LED_QUEUE_CLEAR:
            AppTaskLed_clear_queue( me );
            return 0;

        case LED_QUEUE_START: {
            SyncTimestampEvent *ste = (SyncTimestampEvent *)e;

            if( ste )
            {
                uint32_t epoch_ms = ste->epoch;
                led_interpolator_set_epoch_reference( epoch_ms );
                STATE_TRAN( AppTaskLed_active );
            }
        }
            return 0;

        case LED_REQUEST_MANUAL_CONTROL:
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
            // Run the state-machine loop at 1kHz
            hal_systick_hook( 1, led_interpolator_process );

            AppTaskLed_commit_queued_fade( me );

            if( led_interpolator_is_ready_for_next() )
            {
                stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
            }
            return 0;

        case STATE_STEP1_SIGNAL:
            AppTaskLed_commit_queued_fade( me );
            return 0;

        case ANIMATION_COMPLETE: {
            // the LED interpolation engine has completed the animation execution,
            // loop around to process another event, or go back to inactive and wait for sync
            if( eventQueueUsed( &me->super.requestQueue ) )
            {
                StateEvent *next = eventQueuePeek( &me->super.requestQueue );
                ASSERT( next );
                LightingPlannerEvent *lpe            = (LightingPlannerEvent *)next;
                Fade_t               *next_animation = &lpe->animation;

                if( next_animation->duration )
                {
                    stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
                }
                else
                {
                    STATE_TRAN( AppTaskLed_inactive );
                }
            }
            else
            {
                STATE_TRAN( AppTaskLed_inactive );
            }

            return 0;
        }

        case LED_QUEUE_ADD:
            AppTaskLed_add_event_to_queue( me, e );
            return 0;

        case LED_QUEUE_CLEAR:
            AppTaskLed_clear_queue( me );
            STATE_TRAN( AppTaskLed_inactive );
            return 0;

        case STATE_EXIT_SIGNAL:
            led_interpolator_stop();
            hal_systick_unhook( led_interpolator_process );

            eventTimerStopIfActive( &me->timer1 );
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

            return 0;

        case LED_RELEASE_MANUAL_CONTROL:
            STATE_TRAN( AppTaskLed_inactive );
            return 0;

        case LED_MANUAL_SET: {
            LightingManualEvent *lme = (LightingManualEvent *)e;

            if( lme )
            {
                led_enable( lme->enabled );    // Turn off the LED if the user isn't wanting it on

                if( lme->enabled )
                {
                    GenericColour_t output_values = { 0.0f, 0.0f, 0.0f };

                    output_values.x = (float)lme->colour.red / 0xFFFFU;
                    output_values.y = (float)lme->colour.green / 0xFFFFU;
                    output_values.z = (float)lme->colour.blue / 0xFFFFU;

                    // Set the LED channel values in RGB percentages [0.0f -> 1.0f]
                    led_set( output_values.x, output_values.y, output_values.z );
                }
            }
        }
            return 0;

        case STATE_EXIT_SIGNAL:

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

    // update UI with queue content count
    user_interface_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
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
        // queue full, clearly the input processor isn't abiding by the spec.
        user_interface_report_error( "LED Queue Full" );
    }

    user_interface_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

PRIVATE void AppTaskLed_commit_queued_fade( AppTaskLed *me )
{
    if( led_interpolator_is_ready_for_next()
        && eventQueueUsed( &me->super.requestQueue ) )
    {
        StateEvent *next = eventQueueGet( &me->super.requestQueue );
        ASSERT( next );

        LightingPlannerEvent *lpe            = (LightingPlannerEvent *)next;
        Fade_t               *next_animation = &lpe->animation;

        if( next_animation->duration )
        {
            // Add the valid lighting 'fade' animation to the ping-pong buffer
            led_interpolator_set_objective( next_animation );
            led_interpolator_start();

            eventPoolGarbageCollect( (StateEvent *)next );
        }
    }

    user_interface_set_led_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* ----- End ---------------------------------------------------------------- */
