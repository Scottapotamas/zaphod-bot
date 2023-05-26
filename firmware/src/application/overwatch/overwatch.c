/* -------------------------------------------------------------------------- */

#include "overwatch.h"
#include "mode_mediator.h"

#include "simple_state_machine.h"
#include "signals.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

#include "buzzer.h"

#include "request_handler.h"
#include "path_interpolator.h"
#include "point_follower.h"
#include "effector.h"

#include "led.h"
#include "led_interpolator.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

typedef enum
{
    OVERWATCH_DISARMED,
    OVERWATCH_ARMING,
    OVERWATCH_ARMED,
    OVERWATCH_DISARMING,
    OVERWATCH_EMERGENCY_STOP,
} OverwatchState_t;

typedef struct
{
    OverwatchState_t previousState;
    OverwatchState_t currentState;
    OverwatchState_t nextState;

    bool requested_arming;
    bool effector_home;

    bool servo_active[4];   // keep track of servo active/disarmed
    // TODO: this is actually disarmed/armed - there's no homing/error recovery consideration right now

} Overwatch_t;

PRIVATE SemaphoreHandle_t xOverwatchNotifySemaphore;
PRIVATE TimerHandle_t xOverwatchStimulusTimer;

PRIVATE Overwatch_t supervisor_state = { 0 };

PRIVATE Subject commands = { 0 };
PRIVATE Observer events = { 0 };

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_events_callback(ObserverEvent_t event, EventData eData, void *context);

PRIVATE void overwatch_publish_main_state( void );

PRIVATE void overwatch_publish_mode_state( void );

PRIVATE void overwatch_timer_callback( TimerHandle_t xTimer );

PRIVATE void overwatch_estop_in( uint32_t timeout_ms );

PRIVATE void overwatch_estop_revoke_promise( void );

/* -------------------------------------------------------------------------- */

PUBLIC void overwatch_init( void )
{
//    memset( &supervisor_state, 0, sizeof( Overwatch_t ) );
    Overwatch_t *me = &supervisor_state;
    STATE_INIT_INITIAL(OVERWATCH_DISARMED);

    xOverwatchNotifySemaphore = xSemaphoreCreateBinary();
    ENSURE( xOverwatchNotifySemaphore );

    mode_mediator_init();

    // Setup command generation subject
    subject_init( &commands );

    // Setup subscriptions to events
    observer_init( &events, overwatch_events_callback, NULL );

    // User requests
    observer_subscribe( &events, FLAG_ARM );
    observer_subscribe( &events, FLAG_DISARM );
    observer_subscribe( &events, FLAG_ESTOP );
    observer_subscribe( &events, FLAG_REHOME );
    observer_subscribe( &events, FLAG_MODE_REQUEST );
    observer_subscribe( &events, FLAG_SYNC_EPOCH );
    observer_subscribe( &events, FLAG_REQUEST_QUEUE_CLEAR );

    // Subsystem state updates
    observer_subscribe( &events, SERVO_STATE );
    observer_subscribe( &events, EFFECTOR_NEAR_HOME );

    observer_subscribe( &events, FLAG_EFFECTOR_VIOLATION );
    observer_subscribe( &events, FLAG_PLANNER_VIOLATION );

    // TODO the observer shouldn't be subscribing to position and pushing data downwards to pathing tasks
    observer_subscribe( &events, EFFECTOR_POSITION );

}

/* -------------------------------------------------------------------------- */

PUBLIC Subject * overwatch_get_subject( void )
{
    return &commands;
}

/* -------------------------------------------------------------------------- */

PUBLIC Observer * overwatch_get_observer( void )
{
    return &events;
}

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_events_callback(ObserverEvent_t event, EventData eData, void *context)
{
    Overwatch_t *me = &supervisor_state;

    switch( event )
    {
        case FLAG_ARM:
            me->requested_arming = true;
            break;

        case FLAG_DISARM:
            me->requested_arming = false;
            break;

        // TODO: should effector/planner violations be handled differently from eSTOP?
        case FLAG_EFFECTOR_VIOLATION:
        case FLAG_PLANNER_VIOLATION:
        case FLAG_ESTOP:
            STATE_NEXT(OVERWATCH_EMERGENCY_STOP);
            break;

        case FLAG_REHOME:
            mode_mediator_request_rehome();
            break;

        case SERVO_STATE:   // A servo changed state...
            // TODO don't use hardcoded SERVO_STATE_ACTIVE value of 7 in state check
            me->servo_active[eData.stamped.index] = ( eData.stamped.data.u32 == 7);
            break;

        case FLAG_SYNC_EPOCH:
            // TODO: these tasks should catch the epoch themselves
            path_interpolator_set_epoch_reference( eData.stamped.data.u32 );
            led_interpolator_set_epoch_reference( eData.stamped.data.u32 );
            break;

        case FLAG_REQUEST_QUEUE_CLEAR:
            // TODO: these tasks should catch the clear themselves
            request_handler_clear( REQUEST_HANDLER_MOVES );
            request_handler_clear( REQUEST_HANDLER_FADES );
            path_interpolator_cleanup();
            led_interpolator_cleanup();
            break;

        case FLAG_MODE_REQUEST:     // UI requested a mode change
            mode_mediator_request_mode( eData.stamped.data.u32 );
            break;

        case EFFECTOR_POSITION:
            // TODO: these tasks should catch position themselves?
            path_interpolator_update_effector_position( eData.s_triple[EVT_X],
                                                        eData.s_triple[EVT_Y],
                                                        eData.s_triple[EVT_Z] );

            point_follower_update_effector_position( eData.s_triple[EVT_X],
                                                     eData.s_triple[EVT_Y],
                                                     eData.s_triple[EVT_Z] );

            // Need to work out a 'not at home' event that's separate
            if( eData.s_triple[EVT_X] != 0 || eData.s_triple[EVT_Y] != 0 || eData.s_triple[EVT_Z] != 0 )
            {
                me->effector_home = false;
                mode_mediator_set_is_homed(false );
            }
            break;

        case EFFECTOR_NEAR_HOME:
            // TODO: handle this behaviour in a cleaner manner?
            me->effector_home = true;
            mode_mediator_set_is_homed(true);
            break;

        default:
            ASSERT(false);
            break;
    }

    // Wake the task up
    xSemaphoreGive( xOverwatchNotifySemaphore );

}

PUBLIC void overwatch_task( void* arg )
{
    Overwatch_t *me = &supervisor_state;

    for(;;)
    {
        // Wait for stimulus
        if( xSemaphoreTake( xOverwatchNotifySemaphore, portMAX_DELAY) )
        {
            // Run the state machine at least once per trigger, more if needed to handle transitions etc
            do {
                switch( me->currentState )
                {
                    case OVERWATCH_DISARMED:
                        STATE_ENTRY_ACTION
                        mode_mediator_armed( false );

                        STATE_TRANSITION_TEST

                        if( me->requested_arming )
                        {
                            STATE_NEXT( OVERWATCH_ARMING );
                        }

                        STATE_EXIT_ACTION
                        mode_mediator_armed( true );

                        STATE_END
                        break;

                    case OVERWATCH_ARMING:
                        STATE_ENTRY_ACTION
                        EventData temp = { 0 };
                        temp.stamped.timestamp = xTaskGetTickCount();
                        subject_notify( &commands, OVERWATCH_SERVO_ENABLE, temp );

                        // A pending eSTOP event acts as a timeout
                        overwatch_estop_in( 5000 );
                        STATE_TRANSITION_TEST

                        // Are the motors/subsystems ready?
                        // TODO consider how to optionally handle the 4-th armed servo?
                        if( me->servo_active[0] && me->servo_active[1] && me->servo_active[2]  )
                        {
                            STATE_NEXT( OVERWATCH_ARMED );
                        }

                        if( !me->requested_arming )
                        {
                            STATE_NEXT( OVERWATCH_DISARMING );
                        }

                        STATE_EXIT_ACTION
                        overwatch_estop_revoke_promise();
                        STATE_END
                        break;

                    case OVERWATCH_ARMED:
                        STATE_ENTRY_ACTION
                        effector_reset();

                        STATE_TRANSITION_TEST

                        if( !me->requested_arming )
                        {
                            STATE_NEXT( OVERWATCH_DISARMING );
                        }

                        STATE_EXIT_ACTION

                        STATE_END
                        break;

                    case OVERWATCH_DISARMING:
                        STATE_ENTRY_ACTION
                        me->requested_arming = false;

                        // Ask the mechanism to go home
                        mode_mediator_request_rehome();

                        STATE_TRANSITION_TEST
                        // Are we home?
                        if( me->effector_home )
                        {
                            STATE_NEXT( OVERWATCH_EMERGENCY_STOP );
                        }

                        STATE_EXIT_ACTION

                        STATE_END
                        break;

                    case OVERWATCH_EMERGENCY_STOP:
                        STATE_ENTRY_ACTION
                        me->requested_arming = false;

                        EventData temp = { 0 };
                        temp.stamped.timestamp = xTaskGetTickCount();
                        subject_notify( &commands, OVERWATCH_SERVO_DISABLE, temp );

                        STATE_TRANSITION_TEST
                        // Are the motors/subsystems safe?
                        // TODO consider how to handle the optional 4-th servo?
                        if( !me->servo_active[0] && !me->servo_active[1] && !me->servo_active[2]  )
                        {
                            STATE_NEXT( OVERWATCH_DISARMED );
                        }
                        STATE_EXIT_ACTION

                        STATE_END
                        break;

                }   // end state machine

            } while( STATE_IS_TRANSITIONING );

            // Run the sub-state machine which manages connections between tasks
            mode_mediator_task();

            // State machine handling is done, update the UI
            overwatch_publish_main_state();
            overwatch_publish_mode_state();
        }

    }   // end task loop
}

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_publish_main_state( void )
{
    // TODO timestamp the event?
    EventData sm_current = { 0 };
    sm_current.stamped.timestamp = xTaskGetTickCount();
    sm_current.stamped.data.u32 = supervisor_state.currentState;
    subject_notify( &commands, OVERWATCH_STATE_UPDATE, sm_current );
}

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_publish_mode_state( void )
{
    // The next state is more accurate for notifications than the current state
    //   when disarmed, the mode state-machine isn't run and stays where it is, but the next
    //   state will be used as the transition when it activates, so show that to the user
    EventData mode_next = { 0 };
    mode_next.stamped.timestamp = xTaskGetTickCount();
    mode_next.stamped.data.u32 = mode_mediator_get_mode();
    subject_notify( &commands, OVERWATCH_MODE_UPDATE, mode_next );
}

/* -------------------------------------------------------------------------- */

// Called by FreeRTOS timer task and used to stimulate the task.
// The task itself sets one-shot or repeating timers as needed.
PRIVATE void overwatch_timer_callback( TimerHandle_t xTimer )
{
    Overwatch_t *me = &supervisor_state;

    // TODO is there a better ESTOP option?
    me->requested_arming = false;
    STATE_NEXT( OVERWATCH_EMERGENCY_STOP );

    xSemaphoreGive( xOverwatchNotifySemaphore );
}

PRIVATE void overwatch_estop_in( uint32_t timeout_ms )
{
    xOverwatchStimulusTimer = xTimerCreate("overseer",
                                            pdMS_TO_TICKS(timeout_ms),
                                            pdFALSE,
                                            0,
                                            overwatch_timer_callback
    );
    REQUIRE( xOverwatchStimulusTimer );
    xTimerStart( xOverwatchStimulusTimer, 0 );
}

PRIVATE void overwatch_estop_revoke_promise( void )
{
    xTimerStop( xOverwatchStimulusTimer, 0 );
}

/* -------------------------------------------------------------------------- */