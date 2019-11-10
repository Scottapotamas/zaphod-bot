/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <event_subscribe.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_times.h"
#include "app_signals.h"
#include "app_events.h"
#include "qassert.h"

#include "app_task_motion.h"

#include "motion_types.h"
#include "path_interpolator.h"
#include "kinematics.h"
#include "clearpath.h"

#include "configuration.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void 	AppTaskMotionConstructor( AppTaskMotion *me );

PRIVATE void 	AppTaskMotion_initial	( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_main		( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_home		( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_inactive	( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_active	( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_recovery	( AppTaskMotion *me, const StateEvent *e );

PRIVATE void    AppTaskMotion_commit_queued_move( AppTaskMotion *me );
PRIVATE void    AppTaskMotion_clear_queue( AppTaskMotion *me );
PRIVATE void    AppTaskMotion_add_event_to_queue( AppTaskMotion *me, const StateEvent *e );

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
appTaskMotionCreate(  AppTaskMotion 	*me,
					  StateEvent        *eventQueueData[ ],
					  const uint8_t     eventQueueSize,
					  StateEvent        *movementQueue[ ],
					  const uint8_t     movementQueueSize)
{
    // Clear all task data
    memset( me, 0, sizeof(AppTaskMotion) );

    // Initialise State Machine State
    AppTaskMotionConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask*)me,
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
PRIVATE void AppTaskMotion_initial( AppTaskMotion *me, const StateEvent *e __attribute__((__unused__)) )
{
    eventSubscribe( (StateTask*)me, MOTION_PREPARE );
    eventSubscribe( (StateTask*)me, MOTION_EMERGENCY );

    eventSubscribe( (StateTask*)me, MOTION_QUEUE_ADD );
    eventSubscribe( (StateTask*)me, MOTION_QUEUE_CLEAR );
    eventSubscribe( (StateTask*)me, MOTION_QUEUE_START );
    eventSubscribe( (StateTask*)me, MOTION_QUEUE_START_SYNC );

    eventSubscribe( (StateTask*)me, PATHING_COMPLETE );

    kinematics_init();
    path_interpolator_init();
    config_set_motion_state( TASKSTATE_MOTION_INITIAL );

    STATE_INIT( &AppTaskMotion_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_main( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            config_set_motion_state( TASKSTATE_MOTION_MAIN );

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
                servo_start(servo);
            }

            config_set_motion_state( TASKSTATE_MOTION_HOME );

        	//check the motors every 500ms to see if they are homed
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( SERVO_HOMING_SUPERVISOR_CHECK_MS ) );

        	return 0;

        case STATE_TIMEOUT1_SIGNAL:

        	//check all the servos have homed successfully
        	me->counter = 0;
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
            {
                me->counter += servo_get_servo_ok(servo);
            }

        	if( me->counter == SERVO_COUNT )
        	{
                eventPublish( EVENT_NEW( StateEvent, MOTION_HOMED ) );
                path_interpolator_set_home();
            	STATE_TRAN( AppTaskMotion_inactive );
        	}
        	else
        	{
        		//allow subsequent homing check retries
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
			eventTimerStopIfActive(&me->timer1);

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
            config_set_motion_state( TASKSTATE_MOTION_INACTIVE );

            // Continuously check that the servo's are still happy while holding position
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( SERVO_HOMING_SUPERVISOR_CHECK_MS ) );

        	return 0;

        case STATE_TIMEOUT1_SIGNAL:
        	//check all the servos are active
        	me->counter = 0;
            for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
            {
                me->counter += servo_get_servo_ok(servo);
            }

        	//a servo has dropped offline (fault or otherwise)
        	if( me->counter != SERVO_COUNT )
        	{
        	    config_report_error("Servo loss");
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

        case MOTION_QUEUE_START:
            STATE_TRAN( AppTaskMotion_active );
            return 0;

        case MOTION_QUEUE_START_SYNC:
        {
            // Check that the ID we got the sync event for matches the current queue head ID
            // TODO support sync events on ID's which aren't the current head
            //      consider searching/ditching events until ID matches?
            StateEvent *pendingMotion = eventQueuePeek(&me->super.requestQueue);

            if (pendingMotion)
            {
                MotionPlannerEvent *ape = (MotionPlannerEvent *) pendingMotion;
                uint16_t id_in_queue = ((Movement_t*)&ape->move)->identifier;
                uint16_t id_requested = ((BarrierSyncEvent*)e)->id;

                if( id_in_queue == id_requested )
                {
                    STATE_TRAN( AppTaskMotion_active );
                }
                else
                {
                    config_report_error("Sync fail - queued ID mismatch");
                }
            }
            else
            {
                config_report_error("Sync fail - nothing queued");
            }

            return 0;
        }

        case MOTION_EMERGENCY:
        	STATE_TRAN( AppTaskMotion_recovery );
			return 0;

		case STATE_EXIT_SIGNAL:
			eventTimerStopIfActive(&me->timer1);
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
            config_set_motion_state( TASKSTATE_MOTION_ACTIVE );
            AppTaskMotion_commit_queued_move( me );

            if( path_interpolator_is_ready_for_next() )
            {
                stateTaskPostReservedEvent(STATE_STEP1_SIGNAL);
            }
            return 0;

        case STATE_STEP1_SIGNAL:
            AppTaskMotion_commit_queued_move( me );
            AppTaskMotion_commit_queued_move( me );

            return 0;

        // todo add motion handler watching on PATHING_STARTED?
        //      possibly not needed...
        case PATHING_COMPLETE:
            {
                // the pathing engine completed movement execution,
                // run another event, or go back to inactive to wait for new instructions
                if( eventQueueUsed( &me->super.requestQueue ) )
                {
                    StateEvent * next = eventQueuePeek( &me->super.requestQueue );
                    ASSERT( next );

                    MotionPlannerEvent *ape = (MotionPlannerEvent*)next;
                    Movement_t *next_move = &ape->move;

                    if( next_move->duration )
                    {
                        // add an item to the queue
                        stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
                    }
                    else
                    {
                        STATE_TRAN( AppTaskMotion_inactive);
                    }
                }
                else
                {
                    STATE_TRAN( AppTaskMotion_inactive);
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
                servo_stop(servo);
            }

		    // Stop the motion interpolation engine
            path_interpolator_stop();

        	//update state for UI
            config_set_motion_state( TASKSTATE_MOTION_RECOVERY );

            // Come back next loop and clear out queue etc
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	// Check the motors to ensure shutdown
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( SERVO_RECOVERY_DWELL_MS ) );

        	return 0;

        case STATE_STEP1_SIGNAL:
            AppTaskMotion_clear_queue( me );
            return 0;

        case STATE_TIMEOUT1_SIGNAL:
        	//check all the servos are not-enabled
        	me->counter = SERVO_COUNT;
        	for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
            {
                me->counter -= !servo_get_servo_ok(servo);
            }

        	if( me->counter <= 0 )
        	{
                eventPublish( EVENT_NEW( StateEvent, MOTION_DISABLED ) );
            	STATE_TRAN( AppTaskMotion_main );
        	}
        	else
        	{
        		if( me->retries++ < SERVO_RECOVERY_RETRIES )
        		{
        			eventTimerStopIfActive(&me->timer1);
                	STATE_TRAN( AppTaskMotion_recovery );
        		}
        		else
                {
        		    ASSERT( true ); // motors aren't shutdown after repeated attempts -> hardfault
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
    if(    path_interpolator_is_ready_for_next()
           && eventQueueUsed(&me->super.requestQueue) )
    {
        // Grab the next event off the queue
        StateEvent *next = eventQueueGet( &me->super.requestQueue );
        ASSERT( next );

        MotionPlannerEvent *mpe = (MotionPlannerEvent*)next;
        Movement_t *next_move = &mpe->move;

        if( next_move->duration )
        {
            // Pass this valid move to the pathing engine, and start it
            path_interpolator_set_next( next_move );
            path_interpolator_start();

            eventPoolGarbageCollect( (StateEvent*)next ); // Remove it from the queue
        }
    }

    // Tell the UI the new queue depth after pulling a move from it
    config_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void AppTaskMotion_clear_queue( AppTaskMotion *me )
{
    // Empty the queue
    StateEvent * next = eventQueueGet( &me->super.requestQueue );
    while( next )
    {
        eventPoolGarbageCollect( (StateEvent*)next );
        next = eventQueueGet( &me->super.requestQueue );
    }

    //update UI with queue content count
    config_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE void AppTaskMotion_add_event_to_queue( AppTaskMotion *me, const StateEvent *e )
{
    //already in motion, so add this one to the queue
    MotionPlannerEvent *mpe = (MotionPlannerEvent*)e;

    ASSERT(mpe->move.duration != 0);

    // Add the movement request to the queue if we have room
    uint8_t queue_usage = eventQueueUsed( &me->super.requestQueue );
    if( queue_usage <= MOVEMENT_QUEUE_DEPTH_MAX )
    {
        int32_t speed = cartesian_move_speed(&mpe->move);

        if( speed < EFFECTOR_SPEED_LIMIT )
        {
            eventQueuePutFIFO( &me->super.requestQueue, (StateEvent*)e );
        }
        else
        {
            config_report_error("Requested illegal speed");
        }
    }
    else
    {
        //queue full, clearly the input motion processor isn't abiding by the spec.
        config_report_error("Motion Queue Full");

        //shutdown
        eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
        STATE_TRAN( AppTaskMotion_recovery );
    }

    config_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
}

/* ----- End ---------------------------------------------------------------- */
