/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_events.h"
#include "app_signals.h"
#include "app_times.h"
#include "event_subscribe.h"
#include "hal_systick.h"
#include "qassert.h"

#include "app_task_motion.h"

#include "clearpath.h"
#include "motion_types.h"
#include "path_interpolator.h"
#include "point_follower.h"
#include "effector.h"

#include "user_interface.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskMotionConstructor( AppTaskMotion *me );

PRIVATE void  AppTaskMotion_initial( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_main( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_home( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_inactive( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_execute_events( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_follow_target( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_recovery( AppTaskMotion *me, const StateEvent *e );

PRIVATE void AppTaskMotion_commit_queued_move( AppTaskMotion *me );
PRIVATE void AppTaskMotion_clear_queue( AppTaskMotion *me );
PRIVATE void AppTaskMotion_add_event_to_queue( AppTaskMotion *me, const StateEvent *e );

typedef enum
{
    TASKSTATE_MOTION_INITIAL = 0,
    TASKSTATE_MOTION_MAIN,
    TASKSTATE_MOTION_HOME,
    TASKSTATE_MOTION_INACTIVE,
    TASKSTATE_MOTION_EXECUTE_EVENTS,
    TASKSTATE_MOTION_FOLLOW_POINT,
    TASKSTATE_MOTION_RECOVERY,
} MotionTaskState_t;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskMotionCreate( AppTaskMotion *me,
                     StateEvent    *eventQueueData[],
                     const uint8_t  eventQueueSize,
                     StateEvent    *movementQueue[],
                     const uint8_t  movementQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof( AppTaskMotion ) );

    // Initialise State Machine State
    AppTaskMotionConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask *)me,
                            eventQueueData,
                            eventQueueSize,
                            movementQueue,
                            movementQueueSize );
}

/* ----- Private Functions -------------------------------------------------- */

// State Machine Construction
PRIVATE void AppTaskMotionConstructor( AppTaskMotion *me )
{
    stateTaskCtor( &me->super, (State)&AppTaskMotion_initial );
}

/* -------------------------------------------------------------------------- */

// State Machine Initial State
PRIVATE void AppTaskMotion_initial( AppTaskMotion *me, const StateEvent *e __attribute__( ( __unused__ ) ) )
{
    eventSubscribe( (StateTask *)me, MOTION_PREPARE );
    eventSubscribe( (StateTask *)me, MOTION_EMERGENCY );

    eventSubscribe( (StateTask *)me, MOTION_FOLLOWER_START );
    eventSubscribe( (StateTask *)me, MOTION_FOLLOWER_STOP );

    eventSubscribe( (StateTask *)me, MOTION_QUEUE_START );
    eventSubscribe( (StateTask *)me, MOTION_QUEUE_ADD );
    eventSubscribe( (StateTask *)me, MOTION_QUEUE_CLEAR );
    eventSubscribe( (StateTask *)me, PATHING_COMPLETE );

    eventSubscribe( (StateTask *)me, TRACKED_TARGET_REQUEST );

    effector_init();

    path_interpolator_init( PATH_INTERPOLATOR_DELTA );
    point_follower_init( POINT_FOLLOWER_DELTA );

    user_interface_set_motion_state( TASKSTATE_MOTION_INITIAL );

    STATE_INIT( &AppTaskMotion_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_main( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_motion_state( TASKSTATE_MOTION_MAIN );

            return 0;

        case MOTION_PREPARE:
            STATE_TRAN( AppTaskMotion_home );
            return 0;

        case MOTION_EMERGENCY:
            // TODO: why do we hit this on startup?
            STATE_TRAN( AppTaskMotion_recovery );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_home( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

            me->retries = 0;

            // Reset the motors and let them home
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo <= _CLEARPATH_3; servo++ )
            {
                servo_start( servo );
            }

            user_interface_set_motion_state( TASKSTATE_MOTION_HOME );

            // Check the motors every 500ms to see if they are homed
            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( SERVO_HOMING_SUPERVISOR_CHECK_MS ) );

            return 0;

        case STATE_TIMEOUT1_SIGNAL:

            // Check all the servos have homed successfully by polling their manager for a 'all good' condition
            me->counter = 0;
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo <= _CLEARPATH_3; servo++ )
            {
                me->counter += servo_get_servo_ok( servo );

                // The servo is/recently entered an error recovery state
                if( servo_get_servo_did_error( servo ) )
                {
                    // Bail out of the mechanism homing process entirely
                    // while catching the error and re-requesting the servo to home would be nice, we let the user do that
                    eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
                    STATE_TRAN( AppTaskMotion_recovery );
                    break;
                }
            }

            if( me->counter == 3 )
            {
                eventPublish( EVENT_NEW( StateEvent, MOTION_HOMED ) );
                effector_set_home();
                STATE_TRAN( AppTaskMotion_inactive );
            }
            else
            {
                // Allow subsequent homing check retries
                if( me->retries++ > SERVO_HOMING_SUPERVISOR_RETRIES )
                {
                    eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
                    STATE_TRAN( AppTaskMotion_recovery );
                }
            }

            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskMotion_recovery );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );

            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_inactive( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_motion_state( TASKSTATE_MOTION_INACTIVE );

            // Continuously check that the servos are still happy while holding position
            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( SERVO_HOMING_SUPERVISOR_CHECK_MS ) );

            return 0;

        case STATE_TIMEOUT1_SIGNAL:
            // Check all the servos are active
            me->counter = 0;
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
            {
                me->counter += servo_get_servo_ok( servo );
            }

            // A servo has dropped offline (fault or otherwise)
            if( me->counter != servo_get_configured_count() )
            {
                user_interface_report_error( "Servo loss" );
                eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
                STATE_TRAN( AppTaskMotion_recovery );
            }
            return 0;

        case MOTION_FOLLOWER_START:
            STATE_TRAN( AppTaskMotion_follow_target );
            return 0;

        case MOTION_QUEUE_START: {
            SyncTimestampEvent *ste = (SyncTimestampEvent *)e;

            if( ste )
            {
                path_interpolator_set_epoch_reference( PATH_INTERPOLATOR_DELTA, ste->epoch );
                STATE_TRAN( AppTaskMotion_execute_events );
            }

            return 0;
        }

        case MOTION_QUEUE_ADD:
            AppTaskMotion_add_event_to_queue( me, e );
            return 0;

        case MOTION_QUEUE_CLEAR:
            AppTaskMotion_clear_queue( me );
            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskMotion_recovery );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_execute_events( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_motion_state( TASKSTATE_MOTION_EXECUTE_EVENTS );

            // Run the state-machine loop at 1kHz
            hal_systick_hook( 1, path_interpolator_process_delta );

            // Commit a movement to an available slot in the path planner and tell it to start
            AppTaskMotion_commit_queued_move( me );

            // Start loading more moves from our queue
            if( path_interpolator_is_ready_for_next( PATH_INTERPOLATOR_DELTA ) )
            {
                stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
            }
            return 0;

        case STATE_STEP1_SIGNAL:
            AppTaskMotion_commit_queued_move( me );
            return 0;

        // TODO: add motion handler watching on PATHING_STARTED?
        //      possibly not needed...
        case PATHING_COMPLETE: {
            uint8_t queue_used = eventQueueUsed( &me->super.requestQueue );

            if( queue_used < MOVEMENT_QUEUE_LOW_WATERMARK )
            {
                eventPublish( EVENT_NEW( StateEvent, MOTION_QUEUE_LOW ) );
            }

            // The pathing engine completed movement execution,
            // run another event, or go back to inactive and wait for new instructions
            if( queue_used )
            {
                StateEvent *next = eventQueuePeek( &me->super.requestQueue );
                ASSERT( next );

                MotionPlannerEvent *ape       = (MotionPlannerEvent *)next;
                Movement_t         *next_move = &ape->move;

                if( next_move->duration )
                {
                    // Add an item to the queue
                    stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
                }
                else
                {
                    STATE_TRAN( AppTaskMotion_inactive );
                }
            }
            else
            {
                STATE_TRAN( AppTaskMotion_inactive );
            }

            return 0;
        }

        case MOTION_QUEUE_ADD:
            AppTaskMotion_add_event_to_queue( me, e );
            return 0;

        case MOTION_QUEUE_CLEAR:
            AppTaskMotion_clear_queue( me );
            STATE_TRAN( AppTaskMotion_inactive );
            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskMotion_recovery );
            return 0;

        case STATE_EXIT_SIGNAL:
            path_interpolator_stop( PATH_INTERPOLATOR_DELTA );
            hal_systick_unhook( path_interpolator_process_delta );

            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_follow_target( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_motion_state( TASKSTATE_MOTION_FOLLOW_POINT );

            hal_systick_hook( 1, point_follower_process_delta );
            point_follower_start( POINT_FOLLOWER_DELTA );
            return 0;

        case TRACKED_TARGET_REQUEST: {
            // Catch the inbound target position event
            TrackedPositionRequestEvent *tpre = (TrackedPositionRequestEvent *)e;

            if( &tpre->target )
            {
                CartesianPoint_t target;
                memcpy( &target, &tpre->target, sizeof( CartesianPoint_t ) );

                point_follower_set_target( POINT_FOLLOWER_DELTA, &target );
            }
        }
            return 0;

        case MOTION_FOLLOWER_STOP:
            STATE_TRAN( AppTaskMotion_inactive );
            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskMotion_recovery );
            return 0;

        case STATE_EXIT_SIGNAL:
            point_follower_stop( POINT_FOLLOWER_DELTA );
            hal_systick_unhook( point_follower_process_delta );

            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_recovery( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

            // Disable the servo motors
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
            {
                servo_stop( servo );
            }

            // Stop the motion interpolation engine
            path_interpolator_stop( PATH_INTERPOLATOR_DELTA );

            // Update state for UI
            user_interface_set_motion_state( TASKSTATE_MOTION_RECOVERY );

            // Come back next loop and clear out queue etc
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            // Check the motors to ensure shutdown
            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( SERVO_RECOVERY_DWELL_MS ) );

            return 0;

        case STATE_STEP1_SIGNAL:
            AppTaskMotion_clear_queue( me );
            return 0;

        case STATE_TIMEOUT1_SIGNAL:
            // Check all the servos are not-enabled
            me->counter = 0;
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
            {
                me->counter += !servo_get_servo_ok( servo );
            }

            if( me->counter == _NUMBER_CLEARPATH_SERVOS )
            {
                eventPublish( EVENT_NEW( StateEvent, MOTION_DISABLED ) );
                STATE_TRAN( AppTaskMotion_main );
            }
            else
            {
                if( me->retries++ < SERVO_RECOVERY_RETRIES )
                {
                    eventTimerStopIfActive( &me->timer1 );
                    STATE_TRAN( AppTaskMotion_recovery );
                }
                else
                {
                    ASSERT( true );    // motors aren't shutdown after repeated attempts -> hard-fault
                }
            }

            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE void AppTaskMotion_commit_queued_move( AppTaskMotion *me )
{
    // Check for pending events in the queue, and the pathing engine is able to accept one
    if( path_interpolator_is_ready_for_next( PATH_INTERPOLATOR_DELTA )
        && eventQueueUsed( &me->super.requestQueue ) )
    {
        // Grab the next event off the queue
        StateEvent *next = eventQueueGet( &me->super.requestQueue );
        ASSERT( next );

        MotionPlannerEvent *mpe       = (MotionPlannerEvent *)next;
        Movement_t         *next_move = &mpe->move;

        if( next_move->duration )
        {
            // Pass this valid move to the pathing engine, and start it
            path_interpolator_set_next( PATH_INTERPOLATOR_DELTA, next_move );
            path_interpolator_start( PATH_INTERPOLATOR_DELTA );

            eventPoolGarbageCollect( (StateEvent *)next );    // Remove it from the queue
        }
    }

    // Tell the UI the new queue depth after pulling a move from it
    user_interface_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void AppTaskMotion_clear_queue( AppTaskMotion *me )
{
    // Empty the queue
    StateEvent *next = eventQueueGet( &me->super.requestQueue );
    while( next )
    {
        eventPoolGarbageCollect( (StateEvent *)next );
        next = eventQueueGet( &me->super.requestQueue );
    }

    // Update UI with queue content count
    user_interface_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void AppTaskMotion_add_event_to_queue( AppTaskMotion *me, const StateEvent *e )
{
    // Already in motion, so add this one to the queue
    MotionPlannerEvent *mpe = (MotionPlannerEvent *)e;

    ASSERT( mpe->move.duration != 0 );

    // Add the movement request to the queue if we have room
    uint8_t queue_usage = eventQueueUsed( &me->super.requestQueue );
    if( queue_usage <= MOVEMENT_QUEUE_DEPTH_MAX )
    {
        eventQueuePutFIFO( &me->super.requestQueue, (StateEvent *)e );
    }
    else
    {
        // Queue full, clearly the input motion processor isn't abiding by the spec.
        user_interface_report_error( "Motion Queue Full" );

        // Shutdown
        eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
        STATE_TRAN( AppTaskMotion_recovery );
    }

    user_interface_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* ----- End ---------------------------------------------------------------- */
