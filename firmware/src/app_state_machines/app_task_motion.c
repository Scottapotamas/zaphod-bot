/* ----- System Includes ---------------------------------------------------- */

#include <event_subscribe.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_events.h"
#include "app_signals.h"
#include "app_times.h"
#include "qassert.h"

#include "app_task_motion.h"

#include "clearpath.h"
#include "motion_types.h"
#include "path_interpolator.h"
#include "effector.h"

#include "user_interface.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskMotionConstructor( AppTaskMotion *me );

PRIVATE void  AppTaskMotion_initial( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_main( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_home( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_inactive( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE AppTaskMotion_active( AppTaskMotion *me, const StateEvent *e );
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
    TASKSTATE_MOTION_ACTIVE,
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

    eventSubscribe( (StateTask *)me, MOTION_QUEUE_ADD );
    eventSubscribe( (StateTask *)me, MOTION_QUEUE_CLEAR );
    eventSubscribe( (StateTask *)me, MOTION_QUEUE_START );

    eventSubscribe( (StateTask *)me, PATHING_COMPLETE );

    effector_init();
    path_interpolator_init();
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
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
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
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
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

            if( me->counter == SERVO_COUNT )
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

        case MOTION_QUEUE_ADD:
            AppTaskMotion_add_event_to_queue( me, e );
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
            if( me->counter != SERVO_COUNT )
            {
                user_interface_report_error( "Servo loss" );
                eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
                STATE_TRAN( AppTaskMotion_recovery );
            }
            return 0;

        case MOTION_QUEUE_ADD:
            AppTaskMotion_add_event_to_queue( me, e );
            return 0;

        case MOTION_QUEUE_CLEAR:
            AppTaskMotion_clear_queue( me );
            return 0;

        case MOTION_QUEUE_START: {
            SyncTimestampEvent *ste = (SyncTimestampEvent *)e;

            if( ste )
            {
                path_interpolator_set_epoch_reference( ste->epoch );
                STATE_TRAN( AppTaskMotion_active );
            }

            return 0;
        }

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

PRIVATE STATE AppTaskMotion_active( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            user_interface_set_motion_state( TASKSTATE_MOTION_ACTIVE );
            AppTaskMotion_commit_queued_move( me );

            if( path_interpolator_is_ready_for_next() )
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
            // The pathing engine completed movement execution,
            // run another event, or go back to inactive and wait for new instructions
            if( eventQueueUsed( &me->super.requestQueue ) )
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
            path_interpolator_stop();

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

            if( me->counter == SERVO_COUNT )
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
    if( path_interpolator_is_ready_for_next()
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
            path_interpolator_set_next( next_move );
            path_interpolator_start();

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
