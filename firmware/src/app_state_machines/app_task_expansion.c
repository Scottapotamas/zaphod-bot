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

#include "app_task_expansion.h"

#include "clearpath.h"
#include "motion_types.h"
#include "path_interpolator.h"
#include "point_follower.h"
#include "expansion.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskExpansionConstructor( AppTaskExpansion *me );

PRIVATE void  AppTaskExpansion_initial( AppTaskExpansion *me, const StateEvent *e );
PRIVATE STATE AppTaskExpansion_main( AppTaskExpansion *me, const StateEvent *e );
PRIVATE STATE AppTaskExpansion_home( AppTaskExpansion *me, const StateEvent *e );
PRIVATE STATE AppTaskExpansion_inactive( AppTaskExpansion *me, const StateEvent *e );
PRIVATE STATE AppTaskExpansion_execute_events( AppTaskExpansion *me, const StateEvent *e );
PRIVATE STATE AppTaskExpansion_follow_target( AppTaskExpansion *me, const StateEvent *e );
PRIVATE STATE AppTaskExpansion_recovery( AppTaskExpansion *me, const StateEvent *e );

PRIVATE void AppTaskExpansion_commit_queued_move( AppTaskExpansion *me );
PRIVATE void AppTaskExpansion_clear_queue( AppTaskExpansion *me );
PRIVATE void AppTaskExpansion_add_event_to_queue( AppTaskExpansion *me, const StateEvent *e );

typedef enum
{
    TASKSTATE_EXPANSION_INITIAL = 0,
    TASKSTATE_EXPANSION_MAIN,
    TASKSTATE_EXPANSION_HOME,
    TASKSTATE_EXPANSION_INACTIVE,
    TASKSTATE_EXPANSION_EXECUTE_EVENTS,
    TASKSTATE_EXPANSION_FOLLOW_POINT,
    TASKSTATE_EXPANSION_RECOVERY,
} MotionTaskState_t;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskExpansionCreate( AppTaskExpansion *me,
                     StateEvent    *eventQueueData[],
                     const uint8_t  eventQueueSize,
                     StateEvent    *movementQueue[],
                     const uint8_t  movementQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof( AppTaskExpansion ) );

    // Initialise State Machine State
    AppTaskExpansionConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask *)me,
                            eventQueueData,
                            eventQueueSize,
                            movementQueue,
                            movementQueueSize );
}

/* ----- Private Functions -------------------------------------------------- */

// State Machine Construction
PRIVATE void AppTaskExpansionConstructor( AppTaskExpansion *me )
{
    stateTaskCtor( &me->super, (State)&AppTaskExpansion_initial );
}

/* -------------------------------------------------------------------------- */

// State Machine Initial State
PRIVATE void AppTaskExpansion_initial( AppTaskExpansion *me, const StateEvent *e __attribute__( ( __unused__ ) ) )
{
    eventSubscribe( (StateTask *)me, MOTION_PREPARE );
    eventSubscribe( (StateTask *)me, MOTION_EMERGENCY );

    eventSubscribe( (StateTask *)me, MOTION_FOLLOWER_START );
    eventSubscribe( (StateTask *)me, MOTION_FOLLOWER_STOP );

    eventSubscribe( (StateTask *)me, EXPANSION_QUEUE_START );
    eventSubscribe( (StateTask *)me, EXPANSION_QUEUE_ADD );
    eventSubscribe( (StateTask *)me, MOTION_QUEUE_CLEAR );
    eventSubscribe( (StateTask *)me, EXPANSION_MOVE_COMPLETE );

    eventSubscribe( (StateTask *)me, TRACKED_EXTERNAL_SERVO_REQUEST );

    expansion_init();

    path_interpolator_init( PATH_INTERPOLATOR_EXPANSION );
    point_follower_init( POINT_FOLLOWER_EXPANSION );

    STATE_INIT( &AppTaskExpansion_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskExpansion_main( AppTaskExpansion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
//            user_interface_set_motion_state( TASKSTATE_EXPANSION_MAIN );

            return 0;

        case MOTION_PREPARE:
            if( servo_is_configured( _CLEARPATH_4 ) )
            {
                STATE_TRAN( AppTaskExpansion_home );
            }
            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskExpansion_recovery );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskExpansion_home( AppTaskExpansion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

            me->retries = 0;

            // Reset the motor and let it home as needed
            servo_start( _CLEARPATH_4 );

            // Check the motor every 500ms to see if it's homed
            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( SERVO_HOMING_SUPERVISOR_CHECK_MS ) );

            return 0;

        case STATE_TIMEOUT1_SIGNAL:

            me->counter = servo_get_servo_ok( _CLEARPATH_4 );

            // The servo is/recently entered an error recovery state
            if( servo_get_servo_did_error( _CLEARPATH_4 ) )
            {
                // Bail out of the mechanism homing process entirely
                // while catching the error and re-requesting the servo to home would be nice, we let the user do that
                eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
                STATE_TRAN( AppTaskExpansion_recovery );
                break;
            }

            if( me->counter )
            {
                eventPublish( EVENT_NEW( StateEvent, EXPANSION_HOMED ) );
                expansion_set_home();
                STATE_TRAN( AppTaskExpansion_inactive );
            }
            else
            {
                // Allow subsequent homing check retries
                if( me->retries++ > SERVO_HOMING_SUPERVISOR_RETRIES )
                {
                    eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
                    STATE_TRAN( AppTaskExpansion_recovery );
                }
            }

            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskExpansion_recovery );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );

            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskExpansion_inactive( AppTaskExpansion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

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
                eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
                STATE_TRAN( AppTaskExpansion_recovery );
            }
            return 0;

        case MOTION_FOLLOWER_START:
            STATE_TRAN( AppTaskExpansion_follow_target );
            return 0;

        case MOTION_QUEUE_START: {
            SyncTimestampEvent *ste = (SyncTimestampEvent *)e;

            if( ste )
            {
                path_interpolator_set_epoch_reference( PATH_INTERPOLATOR_EXPANSION, ste->epoch );
                STATE_TRAN( AppTaskExpansion_execute_events );
            }

            return 0;
        }

        case EXPANSION_QUEUE_ADD:
            AppTaskExpansion_add_event_to_queue( me, e );
            return 0;

        case MOTION_QUEUE_CLEAR:
            AppTaskExpansion_clear_queue( me );
            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskExpansion_recovery );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskExpansion_execute_events( AppTaskExpansion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

            // Run the state-machine loop at 1kHz
            hal_systick_hook( 1, path_interpolator_process_all );

            // Commit a movement to an available slot in the path planner and tell it to start
            AppTaskExpansion_commit_queued_move( me );

            // Start loading more moves from our queue
            if( path_interpolator_is_ready_for_next( PATH_INTERPOLATOR_EXPANSION ) )
            {
                stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
            }
            return 0;

        case STATE_STEP1_SIGNAL:
            AppTaskExpansion_commit_queued_move( me );
            return 0;

        // TODO: add motion handler watching on PATHING_STARTED?
        //      possibly not needed...
        case EXPANSION_MOVE_COMPLETE: {
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
                    STATE_TRAN( AppTaskExpansion_inactive );
                }
            }
            else
            {
                STATE_TRAN( AppTaskExpansion_inactive );
            }

            return 0;
        }

        case EXPANSION_QUEUE_ADD:
            AppTaskExpansion_add_event_to_queue( me, e );
            return 0;

        case MOTION_QUEUE_CLEAR:
            AppTaskExpansion_clear_queue( me );
            STATE_TRAN( AppTaskExpansion_inactive );
            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskExpansion_recovery );
            return 0;

        case STATE_EXIT_SIGNAL:
            path_interpolator_stop( PATH_INTERPOLATOR_EXPANSION );
            hal_systick_unhook( path_interpolator_process_all );

            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskExpansion_follow_target( AppTaskExpansion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            hal_systick_hook( 1, point_follower_process_all );
            point_follower_start( POINT_FOLLOWER_EXPANSION );
            return 0;

        case TRACKED_EXTERNAL_SERVO_REQUEST:
        {
            // Catch the inbound target angle
            ExpansionServoRequestEvent *esre = (ExpansionServoRequestEvent *)e;

            if( &esre->target )
            {
                CartesianPoint_t target = { 0 };
                target.x = esre->target;
                point_follower_set_target( POINT_FOLLOWER_EXPANSION, &target );
            }
        }
            return 0;

        case MOTION_FOLLOWER_STOP:
            STATE_TRAN( AppTaskExpansion_inactive );
            return 0;

        case MOTION_EMERGENCY:
            STATE_TRAN( AppTaskExpansion_recovery );
            return 0;

        case STATE_EXIT_SIGNAL:
            point_follower_stop( POINT_FOLLOWER_EXPANSION );
            hal_systick_unhook( point_follower_process_all );

            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskExpansion_recovery( AppTaskExpansion *me, const StateEvent *e )
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
            path_interpolator_stop( PATH_INTERPOLATOR_EXPANSION );

            // Come back next loop and clear out queue etc
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            // Check the motors to ensure shutdown
            eventTimerStartEvery( &me->timer1,
                                  (StateTask *)me,
                                  (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                  MS_TO_TICKS( SERVO_RECOVERY_DWELL_MS ) );

            return 0;

        case STATE_STEP1_SIGNAL:
            AppTaskExpansion_clear_queue( me );
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
                STATE_TRAN( AppTaskExpansion_main );
            }
            else
            {
                if( me->retries++ < SERVO_RECOVERY_RETRIES )
                {
                    eventTimerStopIfActive( &me->timer1 );
                    STATE_TRAN( AppTaskExpansion_recovery );
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

PRIVATE void AppTaskExpansion_commit_queued_move( AppTaskExpansion *me )
{
    // Check for pending events in the queue, and the pathing engine is able to accept one
    if( path_interpolator_is_ready_for_next( PATH_INTERPOLATOR_EXPANSION )
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
            path_interpolator_set_next( PATH_INTERPOLATOR_EXPANSION, next_move );
            path_interpolator_start( PATH_INTERPOLATOR_EXPANSION );

            eventPoolGarbageCollect( (StateEvent *)next );    // Remove it from the queue
        }
    }

    // Tell the UI the new queue depth after pulling a move from it
//    user_interface_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void AppTaskExpansion_clear_queue( AppTaskExpansion *me )
{
    // Empty the queue
    StateEvent *next = eventQueueGet( &me->super.requestQueue );
    while( next )
    {
        eventPoolGarbageCollect( (StateEvent *)next );
        next = eventQueueGet( &me->super.requestQueue );
    }

    // Update UI with queue content count
//    user_interface_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void AppTaskExpansion_add_event_to_queue( AppTaskExpansion *me, const StateEvent *e )
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
//        user_interface_report_error( "Motion Queue Full" );

        // Shutdown
        eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
        STATE_TRAN( AppTaskExpansion_recovery );
    }

//    user_interface_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* ----- End ---------------------------------------------------------------- */
