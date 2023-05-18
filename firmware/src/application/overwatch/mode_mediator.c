/* -------------------------------------------------------------------------- */

#include "mode_mediator.h"
#include "simple_state_machine.h"
#include "qassert.h"

#include "buzzer.h"

#include "user_interface.h"
#include "request_handler.h"
#include "path_interpolator.h"
#include "point_follower.h"
#include "effector.h"

#include "led.h"
#include "led_interpolator.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

typedef struct
{
    ControlModes_t previousState;
    ControlModes_t currentState;
    ControlModes_t nextState;

    ControlModes_t requested_mode;
    bool effector_at_home;

} Modes_t;

PRIVATE Modes_t submode_state = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_init( void )
{
    Modes_t *me = &submode_state;
    STATE_INIT_INITIAL(MODE_UNKNOWN);

}

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_request_mode( ControlModes_t request )
{
    Modes_t *me = &submode_state;

    me->requested_mode = request;

    // Checking for change requests outside the state-machine means
    // this check doesn't need to be repeated in every transition test.
    if( request != me->currentState )
    {
        STATE_NEXT( MODE_CHANGE );
    }
}

/* -------------------------------------------------------------------------- */

// TODO: rename/refactor this behaviour
// Allows armed/disarmed states to 'lock out' path execution etc
PUBLIC void mode_mediator_lockout( bool unlocked )
{
    Modes_t *me = &submode_state;

    if( unlocked )
    {
        STATE_NEXT( MODE_CHANGE );
    }
    else
    {
        STATE_NEXT( MODE_UNKNOWN );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_request_rehome( void )
{
    Modes_t *me = &submode_state;

    // Rehoming is the intent to reset the delta inside the active mode.
    // This is logically identical to the teardown-setup steps that changing mode requires
    // As the request mode won't have changed, it'll just go back once done
    STATE_NEXT( MODE_REHOME );
}

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_set_is_homed( bool effector_at_home )
{
    Modes_t *me = &submode_state;

    me->effector_at_home = effector_at_home;
}

/* -------------------------------------------------------------------------- */

PUBLIC ControlModes_t mode_mediator_get_mode( void )
{
    Modes_t *me = &submode_state;

    if( me->currentState == MODE_UNKNOWN || me->currentState == MODE_REHOME )
    {
        return me->requested_mode;
    }

    return me->nextState;
}

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_task( void )
{
    Modes_t *me = &submode_state;

    do {

        // TODO: Control state edge cases to handle
        //       - When disarmed, changing requested state should let us startup straight into the one we wanted?
        //       - When disarming at the higher level, we should transition into unknown/changing to break connections?

        switch( me->currentState )
        {
            case MODE_UNKNOWN:
                STATE_ENTRY_ACTION

                STATE_TRANSITION_TEST

                // transition to the valid control mode

                STATE_EXIT_ACTION

                STATE_END
                break;

            case MODE_REHOME:
            case MODE_CHANGE:
                STATE_ENTRY_ACTION
                // Overwatch manual request generation -> Path interpolator -> Kinematics
                path_interpolator_update_output_callback( effector_request_target );

                buzzer_sound( BUZZER_MODE_CHANGE_NUM, BUZZER_MODE_CHANGE_TONE, BUZZER_MODE_CHANGE_DURATION );

                STATE_TRANSITION_TEST
                // When at home, transition into desired mode
                if( submode_state.effector_at_home )
                {
                    buzzer_sound( BUZZER_MODE_CHANGED_NUM, BUZZER_MODE_CHANGED_TONE, BUZZER_MODE_CHANGED_DURATION );
                    STATE_NEXT( me->requested_mode );
                }
                else    // effector not at home
                {
                    // Ask the mechanism to go home
                    path_interpolator_request_homing_move();
                }

                STATE_EXIT_ACTION
                path_interpolator_cleanup();
                path_interpolator_update_output_callback( NULL );
                STATE_END
                break;

            case MODE_MANUAL:
                STATE_ENTRY_ACTION
                // Telemetry requests -> Path Interpolator -> Kinematics
                user_interface_attach_motion_request_cb( path_interpolator_queue_request );
                path_interpolator_update_output_callback( effector_request_target );

                // Telemetry requests -> LED driver
                user_interface_attach_colour_request_cb( led_request_target );
                STATE_TRANSITION_TEST

                STATE_EXIT_ACTION
                // Cleanup
                path_interpolator_cleanup();
                led_interpolator_cleanup();

                HSIColour_t darkness = { .hue = 0.0f, .saturation = 0.0f, .intensity = 0.0f };
                led_request_target( &darkness );

                // Break connections
                user_interface_attach_motion_request_cb( NULL );
                path_interpolator_update_output_callback( NULL );
                user_interface_attach_colour_request_cb( NULL );
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
                //            demonstration_update_movement_request_callback( path_interpolator_queue_request );
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
                user_interface_attach_motion_request_cb( request_handler_add_movement );

                RequestableCallbackFn move_req_cb;
                move_req_cb.type = CALLBACK_MOVEMENT;
                move_req_cb.fn.move = path_interpolator_queue_request;
                request_handler_attach_output_callback( REQUEST_HANDLER_MOVES, move_req_cb );

                path_interpolator_update_output_callback( effector_request_target );

                // Telemetry requests -> Ordering queue -> LED Interpolator -> Kinematics
                user_interface_attach_lighting_request_cb( request_handler_add_fade );

                RequestableCallbackFn fade_req_cb;
                fade_req_cb.type = CALLBACK_FADE;
                fade_req_cb.fn.fade = led_interpolator_queue_request;
                request_handler_attach_output_callback( REQUEST_HANDLER_FADES, fade_req_cb );

                led_interpolator_update_output_callback( led_request_target );

                STATE_TRANSITION_TEST

                STATE_EXIT_ACTION
                // Cleanup
                request_handler_clear( REQUEST_HANDLER_MOVES );
                request_handler_clear( REQUEST_HANDLER_FADES );

                path_interpolator_cleanup();
                led_interpolator_cleanup();

                HSIColour_t darkness = { .hue = 0.0f, .saturation = 0.0f, .intensity = 0.0f };
                led_request_target( &darkness );

                // Disconnect the subsystems
                RequestableCallbackFn blank_cb = { 0 };

                user_interface_attach_motion_request_cb( NULL );
                request_handler_attach_output_callback( REQUEST_HANDLER_MOVES, blank_cb );
                path_interpolator_update_output_callback( NULL );

                user_interface_attach_lighting_request_cb( NULL );
                request_handler_attach_output_callback( REQUEST_HANDLER_FADES, blank_cb );
                led_interpolator_update_output_callback( NULL );

                STATE_END
                break;

        }   // end state machine

    } while( STATE_IS_TRANSITIONING );

}

/* -------------------------------------------------------------------------- */