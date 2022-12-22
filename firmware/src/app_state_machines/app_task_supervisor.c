/* ----- System Includes ---------------------------------------------------- */

#include <event_subscribe.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_events.h"
#include "app_signals.h"
#include "app_times.h"
#include "global.h"
#include "qassert.h"

#include "app_task_supervisor.h"
#include "app_task_supervisor_private.h"

#include "buzzer.h"
#include "demonstration.h"
#include "path_interpolator.h"
#include "effector.h"
#include "point_follower.h"
#include "sensors.h"
#include "shutter_release.h"
#include "status.h"
#include "user_interface.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskSupervisorCreate( AppTaskSupervisor *me,
                         StateEvent        *eventQueueData[],
                         const uint8_t      eventQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof( AppTaskSupervisor ) );

    // Initialise State Machine State
    AppTaskSupervisorConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask *)me,
                            eventQueueData,
                            eventQueueSize,
                            0,
                            0 );
}

/* ----- Private Functions -------------------------------------------------- */

/* ----- Private Functions -------------------------------------------------- */

// State Machine Construction
PRIVATE void AppTaskSupervisorConstructor( AppTaskSupervisor *me )
{
    stateTaskCtor( &me->super, (State)&AppTaskSupervisor_initial );
}

/* -------------------------------------------------------------------------- */

// State Machine Initial State
PRIVATE void AppTaskSupervisor_initial( AppTaskSupervisor *me,
                                        const StateEvent  *e __attribute__( ( __unused__ ) ) )
{
    button_init( BUTTON_0, AppTaskSupervisorButtonEvent );
    button_init( BUTTON_1, AppTaskSupervisorButtonEvent );
    button_init( BUTTON_EXTERNAL, AppTaskSupervisorButtonEvent );

    // Detect user activities
    eventSubscribe( (StateTask *)me, BUTTON_NORMAL_SIGNAL );
    eventSubscribe( (StateTask *)me, BUTTON_PRESSED_SIGNAL );

    eventSubscribe( (StateTask *)me, MECHANISM_START );
    eventSubscribe( (StateTask *)me, MECHANISM_STOP );
    eventSubscribe( (StateTask *)me, MECHANISM_REHOME );

    eventSubscribe( (StateTask *)me, MOVEMENT_REQUEST );
    eventSubscribe( (StateTask *)me, TRACKED_TARGET_REQUEST );
    eventSubscribe( (StateTask *)me, TRACKED_EXTERNAL_SERVO_REQUEST );

    // motion handler events
    eventSubscribe( (StateTask *)me, MOTION_ERROR );
    eventSubscribe( (StateTask *)me, MOTION_HOMED );
    eventSubscribe( (StateTask *)me, MOTION_DISABLED );

    // operation modes
    eventSubscribe( (StateTask *)me, MODE_TRACK );
    eventSubscribe( (StateTask *)me, MODE_DEMO );
    eventSubscribe( (StateTask *)me, MODE_EVENT );
    eventSubscribe( (StateTask *)me, MODE_MANUAL );

    eventSubscribe( (StateTask *)me, QUEUE_SYNC_START );

    eventSubscribe( (StateTask *)me, CAMERA_CAPTURE );

    // Put this somewhere more suitable
    point_follower_init();

    STATE_INIT( &AppTaskSupervisor_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskSupervisor_main( AppTaskSupervisor *me,
                                      const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL: {
            user_interface_set_main_state( SUPERVISOR_MAIN );

            // start the board hardware sensors
            sensors_enable();

            status_green( false );
            status_yellow( false );

            return 0;
        }

        case BUTTON_NORMAL_SIGNAL:
            switch( ( (ButtonPressedEvent *)e )->id )
            {
                case BUTTON_0:
                    me->selected_control_mode = CONTROL_DEMO;
                    eventPublish( EVENT_NEW( StateEvent, MECHANISM_START ) );
                    return 0;

                case BUTTON_1:
                    eventPublish( EVENT_NEW( StateEvent, MECHANISM_STOP ) );
                    return 0;

                default:
                    break;
            }
            break;

        case BUTTON_PRESSED_SIGNAL:
            switch( ( (ButtonPressedEvent *)e )->id )
            {
                case BUTTON_EXTERNAL:
                    eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
                    return 0;

                default:
                    break;
            }
            break;

        case STATE_INIT_SIGNAL:
            STATE_INIT( &AppTaskSupervisor_disarmed );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

// Start a mechanism startup

PRIVATE STATE AppTaskSupervisor_disarmed( AppTaskSupervisor *me,
                                          const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_IDLE );
            user_interface_set_control_mode( me->selected_control_mode );

            status_green( false );
            status_yellow( false );

            status_external( false );
            return 0;

        case MECHANISM_START:
            STATE_TRAN( AppTaskSupervisor_arm_start );
            return 0;

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;

        case STATE_EXIT_SIGNAL:
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Start a mechanism startup

PRIVATE STATE AppTaskSupervisor_arm_start( AppTaskSupervisor *me,
                                           const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_ARMING );

            status_yellow( true );
            status_external( true );
            buzzer_sound( BUZZER_ARMING_START_NUM, BUZZER_ARMING_START_TONE, BUZZER_ARMING_START_DURATION );

            // request a motion handler homing process
            eventPublish( EVENT_NEW( StateEvent, MOTION_PREPARE ) );

            // timeout incase the motion handler doesn't signal back
            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( SERVO_HOMING_MAX_MS ) );
            return 0;

        case STATE_TIMEOUT1_SIGNAL:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MOTION_HOMED:
            STATE_TRAN( AppTaskSupervisor_arm_success );
            return 0;

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Handle startup errors

PRIVATE STATE AppTaskSupervisor_arm_error( AppTaskSupervisor *me,
                                           const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_ERROR );
            user_interface_set_control_mode( CONTROL_NONE );

            // We only get here when another state thinks the mechanism isn't responding properly
            // so treat it as a high severity error, and trigger e-stop behaviour
            eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );

            // send message to UI
            user_interface_report_error( "Arming Error" );

            buzzer_sound( BUZZER_ARMING_ERROR_NUM, BUZZER_ARMING_ERROR_TONE, BUZZER_ARMING_ERROR_DURATION );

            return 0;

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:

            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Motor arming succeeded

PRIVATE STATE AppTaskSupervisor_arm_success( AppTaskSupervisor *me,
                                             const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            // update state for UI
            user_interface_set_main_state( SUPERVISOR_SUCCESS );

            status_yellow( false );
            status_green( true );
            status_external( true );
            buzzer_sound( BUZZER_ARMING_OK_NUM, BUZZER_ARMING_OK_TONE, BUZZER_ARMING_OK_DURATION );

            // start additional subsystems here as required

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            return 0;

        case STATE_STEP1_SIGNAL:

            switch( me->selected_control_mode )
            {
                case CONTROL_DEMO:
                    STATE_TRAN( AppTaskSupervisor_armed_demo );
                    break;
                case CONTROL_TRACK:
                    STATE_TRAN( AppTaskSupervisor_armed_track );
                    break;

                case CONTROL_EVENT:
                    STATE_TRAN( AppTaskSupervisor_armed_event );
                    break;

                case CONTROL_MANUAL:
                    STATE_TRAN( AppTaskSupervisor_armed_manual );
                    break;

                default:
                    STATE_TRAN( AppTaskSupervisor_disarm_graceful );
                    break;
            }

            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MECHANISM_STOP:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            user_interface_set_control_mode( me->selected_control_mode );
            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Running Supervisor State

PRIVATE STATE AppTaskSupervisor_armed_event( AppTaskSupervisor *me,
                                             const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_ARMED );
            user_interface_set_control_mode( CONTROL_EVENT );

            // set up any recurring monitoring processes

            return 0;

        case MECHANISM_STOP:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MECHANISM_REHOME:
            AppTaskSupervisorPublishRehomeEvent();
            return 0;

        case QUEUE_SYNC_START: {
            // Create sync start events for the motion and led tasks
            SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
            SyncTimestampEvent *led_sync   = EVENT_NEW( SyncTimestampEvent, LED_QUEUE_START );

            // Set 'now' as the reference time from which moves/fades are executed
            timer_ms_stopwatch_start( &motor_sync->epoch );
            timer_ms_stopwatch_start( &led_sync->epoch );

            eventPublish( (StateEvent *)motor_sync );
            eventPublish( (StateEvent *)led_sync );
            return 0;
        }

            // TODO: Remove movement request proxy behaviour through supervisor?
        case MOVEMENT_REQUEST: {
            // catch the inbound movement event
            MotionPlannerEvent *mpe = (MotionPlannerEvent *)e;

            // Create event to pass event for motion handler
            MotionPlannerEvent *motion_request = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

            if( motion_request )
            {
                // copy the movement into the new event for the motion handler.
                memcpy( &motion_request->move, &mpe->move, sizeof( Movement_t ) );
                eventPublish( (StateEvent *)motion_request );
            }
            return 0;
        }

        case CAMERA_CAPTURE: {
            CameraShutterEvent *trigger = (CameraShutterEvent *)e;

            if( trigger )
            {
                // Treat inbound 0 duration captures as a cancel request.
                // Only attempt a capture if we aren't already
                // Tell the UI when these rules are violated
                if( trigger->exposure_time == 0 )
                {
                    shutter_cancel();
                }
                else if( !shutter_is_exposing() )
                {
                    shutter_capture( trigger->exposure_time );
                }
                else
                {
                    user_interface_report_error( "In use. Capture refused" );
                }
            }

            return 0;
        }

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:

            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

PRIVATE STATE AppTaskSupervisor_armed_manual( AppTaskSupervisor *me,
                                              const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_ARMED );
            user_interface_set_control_mode( CONTROL_MANUAL );
            eventPublish( EVENT_NEW( StateEvent, LED_ALLOW_MANUAL_CONTROL ) );

            return 0;

        case MECHANISM_STOP:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MECHANISM_REHOME:
            AppTaskSupervisorPublishRehomeEvent();
            return 0;

        case MOVEMENT_REQUEST: {
            // catch the inbound movement event
            MotionPlannerEvent *mpe = (MotionPlannerEvent *)e;

            // Create event to pass event for motion handler
            MotionPlannerEvent *motion_request = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

            if( motion_request )
            {
                // copy the movement into the new event for the motion handler.
                memcpy( &motion_request->move, &mpe->move, sizeof( Movement_t ) );
                eventPublish( (StateEvent *)motion_request );

                SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
                timer_ms_stopwatch_start( &motor_sync->epoch );
                eventPublish( (StateEvent *)motor_sync );
            }

            return 0;
        }

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventPublish( EVENT_NEW( StateEvent, LED_RESTRICT_MANUAL_CONTROL ) );

            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

PRIVATE STATE AppTaskSupervisor_armed_track( AppTaskSupervisor *me,
                                             const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_ARMED );
            user_interface_set_control_mode( CONTROL_TRACK );

            eventPublish( EVENT_NEW( StateEvent, LED_ALLOW_MANUAL_CONTROL ) );

            // TODO: pushing 0,0,0 like this is dirty
            user_interface_reset_tracking_target();    // entering track mode should always reset position

            // TODO: this needs refactoring as the motion task normally interacts with the path interpolator
            path_interpolator_stop();
            point_follower_start();
            return 0;

        case TRACKED_TARGET_REQUEST: {
            // Catch the inbound target position event
            TrackedPositionRequestEvent *tpre = (TrackedPositionRequestEvent *)e;

            if( &tpre->target )
            {
                CartesianPoint_t target;
                memcpy( &target, &tpre->target, sizeof( CartesianPoint_t ) );

                // TODO: Do we need to start the point follower first/now?
                point_follower_set_target( &target );
            }
        }
            return 0;

        case TRACKED_EXTERNAL_SERVO_REQUEST:
        {
            // Catch the inbound target angle
            ExpansionServoRequestEvent *esre = (ExpansionServoRequestEvent *)e;

            if( &esre->target )
            {
                float target_deg;
                memcpy( &target_deg, &esre->target, sizeof( float ) );

                // TODO check if the expansion servo is running/moving etc

                // Set the target angle of the expansion clearpath servo
                servo_set_target_angle_raw( _CLEARPATH_4, target_deg );
            }
        }
            return 0;

        case MECHANISM_STOP:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MECHANISM_REHOME:
            // TODO: consider how this should be handled?
            AppTaskSupervisorPublishRehomeEvent();
            user_interface_reset_tracking_target();
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            point_follower_stop();
            user_interface_reset_tracking_target();

            eventTimerStopIfActive( &me->timer1 );
            eventPublish( EVENT_NEW( StateEvent, LED_RESTRICT_MANUAL_CONTROL ) );
            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

PRIVATE STATE AppTaskSupervisor_armed_demo( AppTaskSupervisor *me,
                                            const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_ARMED );
            user_interface_set_control_mode( CONTROL_DEMO );

            demonstration_init();

            // Load up a queue of events
            demonstration_prepare_sequence();

            eventTimerStartOnce( &me->timer1,
                                 (StateTask *)me,
                                 (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                 MS_TO_TICKS( 2000 ) );

            return 0;

        case STATE_TIMEOUT1_SIGNAL: {
            // Generate a sync event to start queue execution
            SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
            timer_ms_stopwatch_start( &motor_sync->epoch );
            eventPublish( (StateEvent *)motor_sync );
        }
            return 0;

        case MECHANISM_STOP:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            STATE_TRAN( AppTaskSupervisor_armed_change_mode );
            return 0;

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

PRIVATE STATE AppTaskSupervisor_armed_change_mode( AppTaskSupervisor *me,
                                                   const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_ARMED );
            user_interface_set_control_mode( CONTROL_CHANGING );

            // empty out the queues
            eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
            eventPublish( EVENT_NEW( StateEvent, LED_QUEUE_CLEAR ) );

            buzzer_sound( BUZZER_MODE_CHANGE_NUM, BUZZER_MODE_CHANGE_TONE, BUZZER_MODE_CHANGE_DURATION );

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( 50 ) );
            return 0;

        case STATE_STEP1_SIGNAL: {
            CartesianPoint_t position = effector_get_position();

            // Check to make sure the mechanism is near the home position before changing mode
            if( position.x < MM_TO_MICRONS( 0.1 )
                && position.y < MM_TO_MICRONS( 0.1 )
                && position.z < MM_TO_MICRONS( 0.1 )
                && path_interpolator_get_move_done() )
            {
                switch( me->selected_control_mode )
                {
                    case CONTROL_DEMO:
                        STATE_TRAN( AppTaskSupervisor_armed_demo );
                        break;

                    case CONTROL_TRACK:
                        STATE_TRAN( AppTaskSupervisor_armed_track );
                        break;

                    case CONTROL_EVENT:
                        STATE_TRAN( AppTaskSupervisor_armed_event );
                        break;

                    case CONTROL_MANUAL:
                        STATE_TRAN( AppTaskSupervisor_armed_manual );
                        break;

                    default:
                        STATE_TRAN( AppTaskSupervisor_disarm_graceful );
                        break;
                }
            }
            else
            {
                // request a move to 0,0,0
                MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );
                motev->move.type          = _POINT_TRANSIT;
                motev->move.ref           = _POS_ABSOLUTE;
                motev->move.sync_offset   = 0;
                motev->move.duration      = 800;
                motev->move.num_pts       = 1;
                motev->move.points[0].x   = 0;
                motev->move.points[0].y   = 0;
                motev->move.points[0].z   = 0;

                eventPublish( (StateEvent *)motev );

                SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
                timer_ms_stopwatch_start( &motor_sync->epoch );
                eventPublish( (StateEvent *)motor_sync );
            }

            return 0;
        }

        case STATE_TIMEOUT1_SIGNAL: {
            CartesianPoint_t position = effector_get_position();

            // Check to make sure the mechanism is near the home position before changing mode
            if( position.x < MM_TO_MICRONS( 0.1 )
                && position.y < MM_TO_MICRONS( 0.1 )
                && position.z < MM_TO_MICRONS( 0.1 )
                && path_interpolator_get_move_done() )
            {
                switch( me->selected_control_mode )
                {
                    case CONTROL_DEMO:
                        STATE_TRAN( AppTaskSupervisor_armed_demo );
                        break;

                    case CONTROL_TRACK:
                        STATE_TRAN( AppTaskSupervisor_armed_track );
                        break;

                    case CONTROL_EVENT:
                        STATE_TRAN( AppTaskSupervisor_armed_event );
                        break;

                    case CONTROL_MANUAL:
                        STATE_TRAN( AppTaskSupervisor_armed_manual );
                        break;

                    default:
                        STATE_TRAN( AppTaskSupervisor_disarm_graceful );
                        break;
                }
            }
            else
            {
                // if the effector isn't moving, and isn't home, then issue another homing move
                if( path_interpolator_get_move_done() )
                {
                    // request a move to 0,0,0
                    MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );
                    motev->move.type          = _POINT_TRANSIT;
                    motev->move.ref           = _POS_ABSOLUTE;
                    motev->move.sync_offset   = 0;
                    motev->move.duration      = 800;
                    motev->move.num_pts       = 1;
                    motev->move.points[0].x   = 0;
                    motev->move.points[0].y   = 0;
                    motev->move.points[0].z   = 0;

                    eventPublish( (StateEvent *)motev );
                    SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
                    timer_ms_stopwatch_start( &motor_sync->epoch );
                    eventPublish( (StateEvent *)motor_sync );
                }
            }
            return 0;
        }

        case MODE_TRACK:
            me->selected_control_mode = CONTROL_TRACK;
            return 0;

        case MODE_EVENT:
            me->selected_control_mode = CONTROL_EVENT;
            return 0;

        case MODE_MANUAL:
            me->selected_control_mode = CONTROL_MANUAL;
            return 0;

        case MODE_DEMO:
            me->selected_control_mode = CONTROL_DEMO;
            return 0;

        case MECHANISM_STOP:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            user_interface_set_control_mode( me->selected_control_mode );
            eventTimerStopIfActive( &me->timer1 );

            buzzer_sound( BUZZER_MODE_CHANGED_NUM, BUZZER_MODE_CHANGED_TONE, BUZZER_MODE_CHANGED_DURATION );
            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Start a graceful shutdown process

PRIVATE STATE AppTaskSupervisor_disarm_graceful( AppTaskSupervisor *me,
                                                 const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_main_state( SUPERVISOR_DISARMING );
            user_interface_set_control_mode( me->selected_control_mode );

            // empty out the queues
            eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
            eventPublish( EVENT_NEW( StateEvent, LED_QUEUE_CLEAR ) );

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            // come back and check the position until we are home
            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( 150 ) );

            eventTimerStartOnce( &me->timer2,
                                 (StateTask *)me,
                                 (StateEvent *)&stateEventReserved[STATE_TIMEOUT2_SIGNAL],
                                 MS_TO_TICKS( 5000 ) );
            return 0;

        case STATE_STEP1_SIGNAL: {
            // request a move to 0,0,0
            MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );

            // transit to starting position
            motev->move.type        = _POINT_TRANSIT;
            motev->move.ref         = _POS_ABSOLUTE;
            motev->move.duration    = 1500;
            motev->move.num_pts     = 1;
            motev->move.sync_offset = 0;

            motev->move.points[0].x = 0;
            motev->move.points[0].y = 0;
            motev->move.points[0].z = 0;

            eventPublish( (StateEvent *)motev );
            SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
            timer_ms_stopwatch_start( &motor_sync->epoch );
            eventPublish( (StateEvent *)motor_sync );

            return 0;
        }

        case STATE_TIMEOUT1_SIGNAL: {
            // get global position
            CartesianPoint_t position = effector_get_position();

            // Check to make sure the mechanism is at the home position before disabling servo power
            // Allow a few microns error on position in check
            if( position.x < MM_TO_MICRONS( 0.15 )
                && position.y < MM_TO_MICRONS( 0.15 )
                && position.z < MM_TO_MICRONS( 0.15 ) )
            {
                // Todo use a 'quiet' disarm here rather than the hard emergency shutdown
                eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
            }

            return 0;
        }

        case STATE_TIMEOUT2_SIGNAL: {
            // Didn't see a full disarm within 5 seconds of starting the process, should have homed by now
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;
        }

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            eventTimerStopIfActive( &me->timer2 );
            return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Tell the motion handler to clear queue, queue a new move to home, and start the move
PRIVATE void AppTaskSupervisorPublishRehomeEvent( void )
{
    eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );

    // request a move to 0,0,0
    MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );
    motev->move.type          = _POINT_TRANSIT;
    motev->move.ref           = _POS_ABSOLUTE;
    motev->move.duration      = 1500;
    motev->move.sync_offset   = 0;
    motev->move.num_pts       = 1;

    motev->move.points[0].x = 0;
    motev->move.points[0].y = 0;
    motev->move.points[0].z = 0;

    eventPublish( (StateEvent *)motev );

    SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
    timer_ms_stopwatch_start( &motor_sync->epoch );
    eventPublish( (StateEvent *)motor_sync );
}

/* -------------------------------------------------------------------------- */

/*
 * Button handling publishes event
 */

PRIVATE void AppTaskSupervisorButtonEvent( ButtonId_t        button,
                                           ButtonPressType_t press_type )
{
    if( press_type == BUTTON_PRESS_TYPE_NORMAL )
    {
        ButtonEvent *be = EVENT_NEW( ButtonEvent, BUTTON_NORMAL_SIGNAL );
        if( be )
        {
            be->id         = button;
            be->press_type = press_type;
            eventPublish( (StateEvent *)be );
        }
    }
    else if( press_type == BUTTON_PRESS_TYPE_DOWN )
    {
        ButtonEvent *be = EVENT_NEW( ButtonEvent, BUTTON_PRESSED_SIGNAL );
        if( be )
        {
            be->id         = button;
            be->press_type = press_type;
            eventPublish( (StateEvent *)be );
        }
    }
}
/* ----- End ---------------------------------------------------------------- */
