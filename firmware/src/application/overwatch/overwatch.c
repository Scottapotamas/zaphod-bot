/* -------------------------------------------------------------------------- */

#include "overwatch.h"
#include "simple_state_machine.h"

#include "FreeRTOS.h"
#include "task.h"

//#include "request_handler.h"
//#include "path_interpolator.h"
//#include "effector.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    MODE_UNKNOWN,
    MODE_CHANGE,
    MODE_MANUAL,
    MODE_TRACK,
    MODE_DEMO,
    MODE_EVENT,
} ModeState_t;

typedef struct
{
    ModeState_t previousState;
    ModeState_t currentState;
    ModeState_t nextState;


} Modes_t;

typedef enum
{
    OVERWATCH_DISARMED,
    OVERWATCH_ARMING,
    OVERWATCH_ARMED,
    OVERWATCH_DISARMING,
} OverwatchState_t;

typedef struct
{
    OverwatchState_t previousState;
    OverwatchState_t currentState;
    OverwatchState_t nextState;


} Overwatch_t;

PRIVATE Overwatch_t supervisor_state = { 0 };

PRIVATE Modes_t submode_state = { 0 };

/* -------------------------------------------------------------------------- */

PRIVATE void overwatch_state_ssm( void );

PRIVATE void overwatch_mode_ssm( void );

/* -------------------------------------------------------------------------- */

PUBLIC void overwatch_init( void )
{
//    memset( &supervisor_state, 0, sizeof( Overwatch_t ) );

    // Set initial states
    supervisor_state.previousState = -1;
    supervisor_state.currentState  = (OVERWATCH_DISARMED);
    supervisor_state.nextState     = (OVERWATCH_DISARMED);

    submode_state.previousState = -1;
    submode_state.currentState  = (MODE_UNKNOWN);
    submode_state.nextState     = (MODE_UNKNOWN);


}

/* -------------------------------------------------------------------------- */

PUBLIC void overwatch_task( void* arg )
{

    for(;;)
    {

        // Wait for a subscribed event to arrive


        overwatch_state_ssm();

        vTaskDelay(1);
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

            //                if( request == arm )
            {
                STATE_NEXT( OVERWATCH_ARMING );
            }

            STATE_EXIT_ACTION

            STATE_END
            break;

        case OVERWATCH_ARMING:
            STATE_ENTRY_ACTION
            // Enable all the motors
            STATE_TRANSITION_TEST

            // Are the motors/subsystems ready?
            STATE_NEXT( OVERWATCH_ARMED );

            STATE_EXIT_ACTION

            STATE_END
            break;

        case OVERWATCH_ARMED:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST

            overwatch_mode_ssm();

            //                if( request == disarm )
            {
            }

            STATE_NEXT( OVERWATCH_DISARMING );
            STATE_EXIT_ACTION

            STATE_END
            break;

        case OVERWATCH_DISARMING:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST

            // Are the motors/subsystems safe?
            STATE_NEXT( OVERWATCH_DISARMED );

            STATE_EXIT_ACTION

            STATE_END
            break;
    }   // end state machine
}

PRIVATE void overwatch_mode_ssm( void )
{
    Modes_t *me = &submode_state;
    STATE_INIT_INITIAL( MODE_UNKNOWN );


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
//            path_interpolator_update_output_callback( effector_request_target );

            // Ask the mechanism to go home
            // Movement_t homing_move = { 0 };
            // TODO: write a helper which generates nice homing movements
            //path_interpolator_add_request( homing_move );

            STATE_TRANSITION_TEST
            // When at home, transition into correct mode
            // TODO: catch event notifying us of being at home?
            //          or poll effector_is_near_home()

            // STATE_NEXT( remembered_mode )

            STATE_EXIT_ACTION

//            path_interpolator_update_output_callback( NULL );
            STATE_END
            break;

        case MODE_MANUAL:
            STATE_ENTRY_ACTION
            // Telemetry requests -> Path Interpolator -> Kinematics
//            telemetry_update_movement_request_callback( path_interpolator_add_request );
//            path_interpolator_update_output_callback( effector_request_target );

            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION
            // Cleanup interpolator

            // Break connections
//            telemetry_update_movement_request_callback( NULL );
//            path_interpolator_update_output_callback( NULL );
            STATE_END
            break;

        case MODE_TRACK:
            STATE_ENTRY_ACTION
            // Telemetry request -> Point follower -> Kinematics
//            telemetry_update_follow_request_callback( point_follower_request_position );
//            point_follower_update_output_callback( effector_request_target );

            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION


            // Break connections
//            telemetry_update_follow_request_callback( NULL );
//            point_follower_update_output_callback( NULL );
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
            // Inform the application tasks of their callbacks/queues etc
            // Telemetry requests -> Ordering queue -> Path Interpolator -> Kinematics
//            telemetry_update_movement_request_callback( request_handler_add_movement );
//            request_handler_update_output_queue_ref( path_interpolator_add_request );
//            path_interpolator_update_output_callback( effector_request_target );

            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION
            // Clear queue

            // Cleanup interpolator state

            // Disconnect the subsystems
//            telemetry_update_movement_request_callback( NULL );
//            request_handler_update_output_queue_ref( NULL );
//            path_interpolator_update_output_callback( NULL );
            STATE_END
            break;

    }   // end state machine



}



/* -------------------------------------------------------------------------- */