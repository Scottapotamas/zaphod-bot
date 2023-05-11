/* -------------------------------------------------------------------------- */

#include "overwatch.h"
#include "simple_state_machine.h"
#include "signals.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

#include "hal_gpio.h"

#include "user_interface.h"
#include "request_handler.h"
#include "path_interpolator.h"
#include "point_follower.h"
#include "effector.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

typedef struct
{
    ControlModes_t previousState;
    ControlModes_t currentState;
    ControlModes_t nextState;

    ControlModes_t requested_mode;

} Modes_t;

typedef enum
{
    OVERWATCH_DISARMED,
    OVERWATCH_ARMING,
    OVERWATCH_ARMED,
    OVERWATCH_DISARMING,
    // TODO ESTOP probably sits here as well?
} OverwatchState_t;

typedef struct
{
    OverwatchState_t previousState;
    OverwatchState_t currentState;
    OverwatchState_t nextState;

    bool requested_arming;

    bool servo_active[4];   // keep track of servo active/disarmed
    // TODO: this is actually disarmed/armed - there's no homing/error recovery consideration right now

} Overwatch_t;

PRIVATE SemaphoreHandle_t xOverwatchNotifySemaphore;
PRIVATE TimerHandle_t xOverwatchStimulusTimer;

PRIVATE Overwatch_t supervisor_state = { 0 };

PRIVATE Modes_t submode_state = { 0 };

PRIVATE Subject commands = { 0 };
PRIVATE Observer events = { 0 };

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_events_callback(ObserverEvent_t event, EventData eData, void *context);

PRIVATE void overwatch_timer_callback( TimerHandle_t xTimer );

PRIVATE void overwatch_state_ssm( void );

PRIVATE void overwatch_mode_ssm( void );

PRIVATE void overwatch_publish_main_state( void );

PRIVATE void overwatch_publish_mode_state( void );

/* -------------------------------------------------------------------------- */

PUBLIC void overwatch_init( void )
{
//    memset( &supervisor_state, 0, sizeof( Overwatch_t ) );

    xOverwatchNotifySemaphore = xSemaphoreCreateBinary();
    ENSURE( xOverwatchNotifySemaphore );



    // Set initial states
    supervisor_state.previousState = -1;
    supervisor_state.currentState  = (OVERWATCH_DISARMED);
    supervisor_state.nextState     = (OVERWATCH_DISARMED);

    submode_state.previousState = -1;
    submode_state.currentState  = (MODE_UNKNOWN);
    submode_state.nextState     = (MODE_UNKNOWN);

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

    // Subsystem state updates
    observer_subscribe( &events, SERVO_STATE );
    observer_subscribe( &events, FLAG_EFFECTOR_NEAR_HOME );

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
    switch( event )
    {
        case FLAG_ARM:
            supervisor_state.requested_arming = true;
            xSemaphoreGive( xOverwatchNotifySemaphore );
            break;
        case FLAG_DISARM:
            supervisor_state.requested_arming = false;
            xSemaphoreGive( xOverwatchNotifySemaphore );
            break;
        case FLAG_ESTOP:
            // TODO: a more severe ESTOP handling approach is needed
            supervisor_state.requested_arming = false;
            xSemaphoreGive( xOverwatchNotifySemaphore );
            break;

        case FLAG_REHOME:
            // This is basically a special-case of changing mode to the same target?

            break;

        case SERVO_STATE:   // One of the servos changed state...
            // TODO don't use hardcoded SERVO_STATE_ACTIVE value of 7 in state check
            supervisor_state.servo_active[eData.index] = ( eData.data.u32 == 7);
            xSemaphoreGive( xOverwatchNotifySemaphore );
            break;

        case FLAG_EFFECTOR_NEAR_HOME:
            // TODO: handle this?
            break;

        case FLAG_SYNC_EPOCH:
            // TODO: handle this for manual, event and demo driven modes?
            break;

        case FLAG_MODE_REQUEST:     // UI requested a mode change
            submode_state.requested_mode = eData.data.u32;

            // Handle the request immediately if we aren't running the mode state-machine
            if( supervisor_state.currentState != OVERWATCH_ARMED )
            {
                submode_state.currentState = submode_state.requested_mode;
                overwatch_publish_mode_state();
            }

            xSemaphoreGive( xOverwatchNotifySemaphore );
            break;

        default:
            ASSERT(false);
            break;
    }
}

// Called by FreeRTOS timer task and used to stimulate the task.
// The task itself sets one-shot or repeating timers as needed.
PRIVATE void overwatch_timer_callback( TimerHandle_t xTimer )
{
    Overwatch_t *me = &supervisor_state;

    // TODO is there a better ESTOP option?
    me->requested_arming = false;
    STATE_NEXT( OVERWATCH_DISARMING);

    xSemaphoreGive( xOverwatchNotifySemaphore );
}

PUBLIC void overwatch_task( void* arg )
{
    Overwatch_t *me = &supervisor_state;

    for(;;)
    {
        // Wait for stimulus
        // TODO: set a maximum time here?
        if( xSemaphoreTake( xOverwatchNotifySemaphore, portMAX_DELAY) )
        {
            // Run the state machine at least once per trigger, more if needed to handle transitions etc
            do {
                overwatch_state_ssm();
            } while( STATE_IS_TRANSITIONING );
        }


    }   // end task loop
}

/* -------------------------------------------------------------------------- */

// TODO rename to better reflect 'high-level-ness'
PRIVATE void overwatch_state_ssm( void )
{
    Overwatch_t *me = &supervisor_state;

    // High level armed/disarmed state machine
    switch( me->currentState )
    {
        case OVERWATCH_DISARMED:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST

            if( me->requested_arming )
            {
                STATE_NEXT( OVERWATCH_ARMING );
            }

            STATE_EXIT_ACTION

            STATE_END
            break;

        case OVERWATCH_ARMING:
            STATE_ENTRY_ACTION
            // TODO: Enable all the motors
            EventData temp = { 0 };
            subject_notify( &commands, OVERWATCH_SERVO_ENABLE, temp );

            // A pending eSTOP event acts as a timeout
            xOverwatchStimulusTimer = xTimerCreate("overseer",
                                                    pdMS_TO_TICKS(5000),
                                                    pdFALSE,
                                                    0,
                                                    overwatch_timer_callback
            );
            REQUIRE( xOverwatchStimulusTimer );
            xTimerStart( xOverwatchStimulusTimer, 0 );

            STATE_TRANSITION_TEST

            // Are the motors/subsystems ready?
            // TODO consider how to optionally handle the 4-th armed servo?
            if( me->servo_active[0] && me->servo_active[1] && me->servo_active[2]  )
            {
                effector_set_home();
                STATE_NEXT( OVERWATCH_ARMED );
            }

            STATE_EXIT_ACTION
            xTimerStop( xOverwatchStimulusTimer, 0 );

            STATE_END
            break;

        case OVERWATCH_ARMED:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST

            // Run the sub-state machine which manages connections between tasks
            overwatch_mode_ssm();

            if( !me->requested_arming )
            {
                STATE_NEXT( OVERWATCH_DISARMING );
            }

            STATE_EXIT_ACTION
            // TODO: Tell the sub-mode system to cleanup?
            // Cleanup the mode management SM
            STATE_END
            break;

        case OVERWATCH_DISARMING:
            STATE_ENTRY_ACTION
            EventData temp = { 0 };
            subject_notify( &commands, OVERWATCH_SERVO_DISABLE, temp );

            STATE_TRANSITION_TEST
            // Are the motors/subsystems safe?
            // TODO consider how to optionally handle the 4-th armed servo?
            if( !me->servo_active[0] && !me->servo_active[1] && !me->servo_active[2]  )
            {
                effector_set_home();
                STATE_NEXT( OVERWATCH_DISARMED );
            }
            STATE_EXIT_ACTION

            STATE_END
            break;
    }   // end state machine

    if( !STATE_IS_TRANSITIONING )
    {
        overwatch_publish_main_state();
    }
}

PRIVATE void overwatch_mode_ssm( void )
{
    Modes_t *me = &submode_state;

    // By checking for change requests outside the state-machine,
    // this check doesn't need to be repeated in every transition test.
    if( me->requested_mode != me->currentState )
    {
        STATE_NEXT( MODE_CHANGE );
    }

    do {


    // TODO: Control state edge cases to handle
    //       - When disarmed, changing requested state should let us startup straight into the one we wanted?
    //       - When disarmming at the higher level, we should transition into unknown/changing to break connections?
    //       - Can we use changing state behaviour for re-home events?


    switch( me->currentState )
    {
        case MODE_UNKNOWN:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST

            // transition to the valid control mode

            STATE_EXIT_ACTION

            STATE_END
            break;

        case MODE_CHANGE:
            STATE_ENTRY_ACTION
            // Overwatch manual request generation -> Path interpolator -> Kinematics
            path_interpolator_update_output_callback( effector_request_target );

            // Ask the mechanism to go home
            // Movement_t homing_move = { 0 };
            // TODO: write a helper which generates nice homing movements
            //path_interpolator_add_request( homing_move );

            // buzzer_sound( BUZZER_MODE_CHANGE_NUM, BUZZER_MODE_CHANGE_TONE, BUZZER_MODE_CHANGE_DURATION );

            STATE_TRANSITION_TEST
            // When at home, transition into correct mode
            // TODO: catch event notifying us of being at home?
            //          or poll effector_is_near_home()

            // If it doesn't get there in X time, fire an E-STOP?

            STATE_NEXT( me->requested_mode );

            STATE_EXIT_ACTION

            path_interpolator_update_output_callback( NULL );
            STATE_END
            break;

        case MODE_MANUAL:
            STATE_ENTRY_ACTION
            // Telemetry requests -> Path Interpolator -> Kinematics
            user_interface_attach_motion_request_cb( path_interpolator_add_request );
            path_interpolator_update_output_callback( effector_request_target );
            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION
            // Cleanup interpolator

            // Break connections
            user_interface_attach_motion_request_cb( NULL );
            path_interpolator_update_output_callback( NULL );
            STATE_END
            break;

        case MODE_TRACK:
            STATE_ENTRY_ACTION
            // Telemetry request -> Point follower -> Kinematics
            user_interface_attach_position_request_cb( point_follower_set_target );
            point_follower_update_output_callback( effector_request_target );

            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION

            // Break connections
            user_interface_attach_position_request_cb( NULL );
            point_follower_update_output_callback( NULL );
            STATE_END
            break;

        case MODE_DEMO:
            STATE_ENTRY_ACTION
            // Demo generators -> Path interpolator -> Kinematics
//            demonstration_update_movement_request_callback( path_interpolator_add_request );
//            path_interpolator_update_output_callback( effector_request_target );

            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION

//            demonstration_update_movement_request_callback( NULL );
//            path_interpolator_update_output_callback( NULL );
            STATE_END
            break;

        case MODE_EVENT:
            STATE_ENTRY_ACTION
            hal_gpio_write_pin( _STATUS_2, true );

            // Inform the application tasks of their callbacks/queues etc
            // Telemetry requests -> Ordering queue -> Path Interpolator -> Kinematics
            user_interface_attach_motion_request_cb( request_handler_add_movement );
            request_handler_attach_output_callback( path_interpolator_add_request );
            path_interpolator_update_output_callback( effector_request_target );

            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION
            // Clear queues
            hal_gpio_write_pin( _STATUS_2, false );

            // Cleanup interpolator state

            // Disconnect the subsystems
            user_interface_attach_motion_request_cb( NULL );
            request_handler_attach_output_callback( NULL );
            path_interpolator_update_output_callback( NULL );
            STATE_END
            break;

    }   // end state machine

    } while( STATE_IS_TRANSITIONING );

    if( !STATE_IS_TRANSITIONING )
    {
        overwatch_publish_mode_state();
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_publish_main_state( void )
{
    EventData sm_current = { .data.u32 = supervisor_state.currentState };
    subject_notify( &commands, OVERWATCH_STATE_UPDATE, sm_current );
}

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_publish_mode_state( void )
{
    EventData mode_current = { .data.u32 = submode_state.currentState };
    subject_notify( &commands, OVERWATCH_MODE_UPDATE, mode_current );
}

/* -------------------------------------------------------------------------- */