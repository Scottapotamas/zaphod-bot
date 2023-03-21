/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "app_config.h"
#include "app_events.h"
#include "app_signals.h"
#include "app_times.h"
#include "global.h"
#include "qassert.h"
#include "event_subscribe.h"

#include "app_task_supervisor.h"
#include "app_task_supervisor_private.h"

#include "buzzer.h"
#include "demonstration.h"
#include "path_interpolator.h"
#include "point_follower.h"
#include "effector.h"
#include "sensors.h"
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
#ifndef ESTOP_PENDANT_IS_SMART
    button_init( BUTTON_EXTERNAL, AppTaskSupervisorButtonEvent );
#endif

    // Detect user activities
    eventSubscribe( (StateTask *)me, BUTTON_NORMAL_SIGNAL );
    eventSubscribe( (StateTask *)me, BUTTON_PRESSED_SIGNAL );

    eventSubscribe( (StateTask *)me, MECHANISM_START );
    eventSubscribe( (StateTask *)me, MECHANISM_STOP );
    eventSubscribe( (StateTask *)me, MECHANISM_REHOME );

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
    eventSubscribe( (StateTask *)me, MOTION_QUEUE_ADD );
    eventSubscribe( (StateTask *)me, MOTION_QUEUE_LOW );

    eventSubscribe( (StateTask *)me, DEMO_MODE_CONFIGURATION );

    STATE_INIT( &AppTaskSupervisor_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskSupervisor_main( AppTaskSupervisor *me,
                                      const StateEvent  *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL: {
            me->armed = SUPERVISOR_NONE;
            user_interface_set_main_state( me->armed );

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
                    me->requested_control_mode = CONTROL_DEMO;
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
#ifndef ESTOP_PENDANT_IS_SMART
                case BUTTON_EXTERNAL:
                    eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
                    return 0;
#endif
                default:
                    break;
            }
            break;

        case MODE_MANUAL:
            me->requested_control_mode = CONTROL_MANUAL;
            AppTaskSupervisorProcessModeRequest( me );
            return 0;

        case MODE_TRACK:
            me->requested_control_mode = CONTROL_TRACK;
            AppTaskSupervisorProcessModeRequest( me );
            return 0;

        case MODE_DEMO:
            me->requested_control_mode = CONTROL_DEMO;
            AppTaskSupervisorProcessModeRequest( me );
            return 0;

        case MODE_EVENT:
            me->requested_control_mode = CONTROL_EVENT;
            AppTaskSupervisorProcessModeRequest( me );
            return 0;

        case DEMO_MODE_CONFIGURATION:
        {
            DemoModeConfigurationEvent *config = (DemoModeConfigurationEvent *)e;

            if( config )
            {
                me->requested_demo_program = config->program_index;
            }
        }
            return 0;

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
            me->armed = SUPERVISOR_IDLE;
            user_interface_set_main_state( me->armed );

            status_green( false );
            status_yellow( false );

            status_external( false );
            return 0;

        case MECHANISM_START:
            STATE_TRAN( AppTaskSupervisor_arm_start );
            return 0;

        case STATE_EXIT_SIGNAL:
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
            me->armed = SUPERVISOR_ARMING;
            user_interface_set_main_state( me->armed );

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
            me->armed = SUPERVISOR_ERROR;
            user_interface_set_main_state( me->armed );

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
            me->armed = SUPERVISOR_ARMED;
            user_interface_set_main_state( me->armed );

            status_yellow( false );
            status_green( true );
            status_external( true );
            buzzer_sound( BUZZER_ARMING_OK_NUM, BUZZER_ARMING_OK_TONE, BUZZER_ARMING_OK_DURATION );

            // start additional subsystems here as required

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            return 0;

        case STATE_STEP1_SIGNAL:

            switch( me->requested_control_mode )
            {
                case CONTROL_MANUAL:
                    STATE_TRAN( AppTaskSupervisor_armed_manual );
                    break;

                case CONTROL_TRACK:
                    STATE_TRAN( AppTaskSupervisor_armed_track );
                    break;

                case CONTROL_DEMO:
                    STATE_TRAN( AppTaskSupervisor_armed_demo );
                    break;

                case CONTROL_EVENT:
                    STATE_TRAN( AppTaskSupervisor_armed_event );
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
            me->active_control_mode = CONTROL_EVENT;
            user_interface_set_control_mode( me->active_control_mode );

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
            SyncTimestampEvent *motor_sync     = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
            SyncTimestampEvent *expansion_sync = EVENT_NEW( SyncTimestampEvent, EXPANSION_QUEUE_START );
            SyncTimestampEvent *led_sync       = EVENT_NEW( SyncTimestampEvent, LED_QUEUE_START );

            // Set 'now' as the reference time from which moves/fades are executed
            timer_ms_stopwatch_start( &motor_sync->epoch );
            timer_ms_stopwatch_start( &expansion_sync->epoch );
            timer_ms_stopwatch_start( &led_sync->epoch );

            eventPublish( (StateEvent *)motor_sync );
            eventPublish( (StateEvent *)expansion_sync );
            eventPublish( (StateEvent *)led_sync );
            return 0;
        }

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
            me->active_control_mode = CONTROL_MANUAL;
            user_interface_set_control_mode( me->active_control_mode );

            eventPublish( EVENT_NEW( StateEvent, LED_REQUEST_MANUAL_CONTROL ) );
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

        case MOTION_QUEUE_ADD: {
            // catch the inbound movement event
            MotionPlannerEvent *mpe = (MotionPlannerEvent *)e;

            if( mpe->move.points )
            {
                // Fire a sync event now to trigger the moves immediately
                // the manual mode doesn't really use queuing behaviours
                SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
                timer_ms_stopwatch_start( &motor_sync->epoch );
                eventPublish( (StateEvent *)motor_sync );
            }

            return 0;
        }

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventPublish( EVENT_NEW( StateEvent, LED_RELEASE_MANUAL_CONTROL ) );

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
            me->active_control_mode = CONTROL_TRACK;
            user_interface_set_control_mode( me->active_control_mode );

            eventPublish( EVENT_NEW( StateEvent, MOTION_FOLLOWER_START ) );
            eventPublish( EVENT_NEW( StateEvent, LED_REQUEST_MANUAL_CONTROL ) );

            // TODO: pushing 0,0,0 like this is dirty
            user_interface_reset_tracking_target();    // entering track mode should always reset position

            return 0;

        case MECHANISM_STOP:
            STATE_TRAN( AppTaskSupervisor_disarm_graceful );
            return 0;

        case MOTION_ERROR:
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

        case MECHANISM_REHOME: {
            // TODO: Cleanup needed
            CartesianPoint_t home = { 0, 0, 0 };
            point_follower_set_target( POINT_FOLLOWER_DELTA, &home );

            user_interface_reset_tracking_target();
            return 0;
        }

        case MOTION_DISABLED:
            STATE_TRAN( AppTaskSupervisor_disarmed );
            return 0;

        case STATE_EXIT_SIGNAL:
            user_interface_reset_tracking_target();

            eventPublish( EVENT_NEW( StateEvent, MOTION_FOLLOWER_STOP ) );
            eventPublish( EVENT_NEW( StateEvent, LED_RELEASE_MANUAL_CONTROL ) );

            eventTimerStopIfActive( &me->timer1 );
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
            me->active_control_mode = CONTROL_DEMO;
            user_interface_set_control_mode( me->active_control_mode );

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            // Wait before starting the demo for the first time?
            eventTimerStartOnce( &me->timer1,
                                 (StateTask *)me,
                                 (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                 MS_TO_TICKS( 2000 ) );

            // TODO buzzer notification?
            return 0;

        case STATE_STEP1_SIGNAL:
            // Cleanup any prior demo state, specify which program to run
            demonstration_init( me->requested_demo_program );

            // Feed the movement queue a larger batch of initial moves
            demonstration_enqueue_moves( 6 );
            return 0;

        case MOTION_QUEUE_LOW:
            // Generate/load some events and add them to the motion queue
            demonstration_enqueue_moves( 4 );
            return 0;

        case STATE_TIMEOUT1_SIGNAL: {
            // Generate a sync event to start queue execution
            SyncTimestampEvent *motor_sync = EVENT_NEW( SyncTimestampEvent, MOTION_QUEUE_START );
            timer_ms_stopwatch_start( &motor_sync->epoch );
            eventPublish( (StateEvent *)motor_sync );
        }
            return 0;

        case DEMO_MODE_CONFIGURATION: {
            DemoModeConfigurationEvent *config = (DemoModeConfigurationEvent *)e;

            if( config )
            {
                me->requested_demo_program = config->program_index;
            }

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
        }
            return 0;

        case MECHANISM_REHOME:
            AppTaskSupervisorPublishRehomeEvent();
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
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
            me->active_control_mode = CONTROL_CHANGING;
            user_interface_set_control_mode( me->active_control_mode );

            // empty out the queues
            eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
            eventPublish( EVENT_NEW( StateEvent, LED_QUEUE_CLEAR ) );

            buzzer_sound( BUZZER_MODE_CHANGE_NUM, BUZZER_MODE_CHANGE_TONE, BUZZER_MODE_CHANGE_DURATION );

            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( 50 ) );

            eventTimerStartOnce( &me->timer2,
                                 (StateTask *)me,
                                 (StateEvent *)&stateEventReserved[STATE_TIMEOUT2_SIGNAL],
                                 MS_TO_TICKS( 5000 ) );

            return 0;

        case STATE_TIMEOUT1_SIGNAL:
            // Check to make sure the mechanism is near the home position before changing mode
            if( effector_is_near_home() && path_interpolator_get_move_done( PATH_INTERPOLATOR_DELTA ) )
            {
                switch( me->requested_control_mode )
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
                if( path_interpolator_get_move_done( PATH_INTERPOLATOR_DELTA ) )
                {
                    AppTaskSupervisorPublishRehomeEvent();
                }
            }
            return 0;

        case STATE_TIMEOUT2_SIGNAL:
            // Didn't see valid behaviour after 5 seconds, something must be wrong
            STATE_TRAN( AppTaskSupervisor_arm_error );
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
            eventTimerStopIfActive( &me->timer1 );
            eventTimerStopIfActive( &me->timer2 );

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
            me->armed = SUPERVISOR_DISARMING;
            user_interface_set_main_state( me->armed );

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

        case STATE_STEP1_SIGNAL:
            AppTaskSupervisorPublishRehomeEvent();
            return 0;

        case STATE_TIMEOUT1_SIGNAL:
            // Check to make sure the mechanism is at the home position before disabling servo power
            // Allow a few microns error on position in check
            if( effector_is_near_home() )
            {
                // TODO: use a 'quiet' disarm here rather than the hard emergency shutdown
                eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
            }

            return 0;

        case STATE_TIMEOUT2_SIGNAL:
            // Didn't see a full disarm within 5 seconds of starting the process, should have homed by now
            STATE_TRAN( AppTaskSupervisor_arm_error );
            return 0;

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

// In normal operation, asks the supervisor to change into the new mode.
// If we're disarmed or pre-arm, update the target mode directly
PRIVATE void AppTaskSupervisorProcessModeRequest( AppTaskSupervisor *me )
{
    // Don't transition states if already there
    if( me->requested_control_mode == me->active_control_mode )
    {
        return;
    }

    // When disarmed, full state transitions aren't needed
    if( me->armed != SUPERVISOR_ARMED )
    {
        me->active_control_mode = me->requested_control_mode;
        user_interface_set_control_mode( me->active_control_mode );
        return;
    }

    STATE_TRAN( AppTaskSupervisor_armed_change_mode );
}

/* -------------------------------------------------------------------------- */

// Tell the motion handler to clear queue, queue a new move to home, and start the move
PRIVATE void AppTaskSupervisorPublishRehomeEvent( void )
{
    eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_CLEAR ) );
    eventPublish( EVENT_NEW( StateEvent, LED_QUEUE_CLEAR ) );

    // request a move to 0,0,0
    MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_QUEUE_ADD );
    motev->move.type          = _POINT_TRANSIT;
    motev->move.ref           = _POS_ABSOLUTE;
    motev->move.duration      = 800;
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
