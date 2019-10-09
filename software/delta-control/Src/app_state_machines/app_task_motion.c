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

        	//reset the motors and let them home
        	servo_start( _CLEARPATH_1 );
        	servo_start( _CLEARPATH_2 );
        	servo_start( _CLEARPATH_3 );
#ifdef EXPANSION_SERVO
        	servo_start( _CLEARPATH_4 );
#endif

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
        	me->counter += servo_get_valid_home( _CLEARPATH_1 );
        	me->counter += servo_get_valid_home( _CLEARPATH_2 );
        	me->counter += servo_get_valid_home( _CLEARPATH_3 );
#ifdef EXPANSION_SERVO
        	me->counter += servo_get_valid_home( _CLEARPATH_4 );
#endif

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
        	{
				MotionPlannerEvent *mpe = (MotionPlannerEvent*)e;

				// Add the movement request to the queue if we have room
				uint8_t queue_usage = eventQueueUsed( &me->super.requestQueue );
				if( queue_usage <= MOVEMENT_QUEUE_DEPTH_MAX )
				{
					if( mpe->move.duration)
					{
						eventQueuePutFIFO( &me->super.requestQueue, (StateEvent*)e );
					}
				}

				//update UI with queue depth
				config_set_motion_queue_depth( queue_usage );
        	}
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

            // Check for queued events
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            // Continuously check that the servo's are still happy while holding position
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( SERVO_HOMING_SUPERVISOR_CHECK_MS ) );

        	return 0;

        case STATE_STEP1_SIGNAL:
            {
                // Check the queue for pending movements, tell the supervisor the next value in the queue
                StateEvent *pendingMotion = eventQueuePeek(&me->super.requestQueue);

                if (pendingMotion)
                {
                    MotionPlannerEvent *ape = (MotionPlannerEvent *) pendingMotion;
                    uint16_t id_in_queue = ((Movement_t*)&ape->move)->identifier;

                    if( id_in_queue )
                    {
                        // tell the supervisor what the next move in the queue in
                        BarrierSyncEvent *motor_sync_next = EVENT_NEW( BarrierSyncEvent, QUEUE_SYNC_MOTION_NEXT );
                        motor_sync_next->id = id_in_queue;
                        eventPublish( (StateEvent*)motor_sync_next );
                    }
                }

                // update the UI with the queue depth
                config_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
            }
            return 0;

        case STATE_TIMEOUT1_SIGNAL:
        	//check all the servos are active
        	me->counter = 0;
        	me->counter += servo_get_valid_home( _CLEARPATH_1 );
        	me->counter += servo_get_valid_home( _CLEARPATH_2 );
        	me->counter += servo_get_valid_home( _CLEARPATH_3 );
#ifdef EXPANSION_SERVO
        	me->counter += servo_get_valid_home( _CLEARPATH_4 );
#endif

        	//a servo has dropped offline (fault or otherwise)
        	if( me->counter != SERVO_COUNT )
        	{
                eventPublish( EVENT_NEW( StateEvent, MOTION_ERROR ) );
        		STATE_TRAN( AppTaskMotion_recovery );
        	}
			return 0;

        case MOTION_QUEUE_ADD:
			{
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

                        if( mpe->move.identifier == 0 )
                        {
                            STATE_TRAN( AppTaskMotion_active );
                        }
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
			return 0;

        case MOTION_QUEUE_CLEAR:
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
            return 0;

        case MOTION_QUEUE_START:
            me->identifier_to_execute = 0;  // manually added moves have no identifer check, manual starts don't need sync
            STATE_TRAN( AppTaskMotion_active );
            return 0;

        case MOTION_QUEUE_START_SYNC:
            // Grab the inbound requested id as part of barrier handling
            me->identifier_to_execute = ((BarrierSyncEvent*)e)->id;
            STATE_TRAN( AppTaskMotion_active );
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

PRIVATE STATE AppTaskMotion_active( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
		case STATE_ENTRY_SIGNAL:
            config_set_motion_state( TASKSTATE_MOTION_ACTIVE );
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

            return 0;

        case STATE_STEP1_SIGNAL:
            {
                // check for pending events in the queue
                if( eventQueueUsed( &me->super.requestQueue ) )
                {
                    // grab the next event off the queue
                    StateEvent *next = eventQueueGet( &me->super.requestQueue );

                    // start up the pathing engine
                    if(next)
                    {
                        MotionPlannerEvent *ape = (MotionPlannerEvent*)next;
                        Movement_t *next_move = &ape->move;

                        if( me->identifier_to_execute == 0 || me->identifier_to_execute >= next_move->identifier )
                        {
                            path_interpolator_set_objective(next_move);

                            // remove it from the queue
                            eventPoolGarbageCollect( (StateEvent*)next );
                        }
                    }
                }
                else
                {
                    // no events in the queue
                    STATE_TRAN( AppTaskMotion_inactive );
                }

                config_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
                return 0;
            }

        case PATHING_COMPLETE:
            {
                // the pathing engine has completed the movement execution,
                // loop around to process another event, or go back to inactive and wait for sync

                if( eventQueueUsed( &me->super.requestQueue ) )
                {
                    StateEvent * next = eventQueuePeek( &me->super.requestQueue );
                    MotionPlannerEvent *ape = (MotionPlannerEvent*)next;
                    Movement_t *next_move = &ape->move;

                    if(next)
                    {
                        if( me->identifier_to_execute == 0 || me->identifier_to_execute >= next_move->identifier )
                        {
                            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );
                        }
                        else
                        {
                            STATE_TRAN( AppTaskMotion_inactive);
                        }
                    }
                }
                else
                {
                    STATE_TRAN( AppTaskMotion_inactive);
                }

                config_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
                return 0;
            }

        case MOTION_QUEUE_ADD:
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
        	return 0;

        case MOTION_QUEUE_CLEAR:
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

                STATE_TRAN( AppTaskMotion_inactive );   //go back to idle, no point being active with a drained queue
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


PRIVATE STATE AppTaskMotion_recovery( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
		case STATE_ENTRY_SIGNAL:

		    // disable the motion interpolation engine
            path_interpolator_stop();

        	//disable servos
        	servo_stop( _CLEARPATH_1 );
        	servo_stop( _CLEARPATH_2 );
        	servo_stop( _CLEARPATH_3 );
        #ifdef EXPANSION_SERVO
        	servo_stop( _CLEARPATH_4 );
        #endif

        	//update state for UI
            config_set_motion_state( TASKSTATE_MOTION_RECOVERY );

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	//check the motors to ensure shutdown
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( SERVO_RECOVERY_DWELL_MS ) );

        	return 0;

        case STATE_STEP1_SIGNAL:
        	{
				//clear out any pending movements from the queue
				StateEvent * next = eventQueueGet( &me->super.requestQueue );
				while( next )
				{
					eventPoolGarbageCollect( (StateEvent*)next );
					next = eventQueueGet( &me->super.requestQueue );
				}

				//update UI with queue content count
				config_set_motion_queue_depth( eventQueueUsed( &me->super.requestQueue ) );
        	}
            return 0;

        case STATE_TIMEOUT1_SIGNAL:

        	//check all the servos are not enabled
        	me->counter = SERVO_COUNT;
        	me->counter -= !servo_get_valid_home( _CLEARPATH_1 );
        	me->counter -= !servo_get_valid_home( _CLEARPATH_2 );
        	me->counter -= !servo_get_valid_home( _CLEARPATH_3 );
#ifdef EXPANSION_SERVO
        	me->counter -= servo_get_valid_home( _CLEARPATH_4 );
#endif

        	if( me->counter <= 0 )
        	{
                eventPublish( EVENT_NEW( StateEvent, MOTION_DISABLED ) );
            	STATE_TRAN( AppTaskMotion_main );
        	}
        	else
        	{
        		if( me->retries++ > SERVO_RECOVERY_RETRIES )
        		{
        			eventTimerStopIfActive(&me->timer1);
                	STATE_TRAN( AppTaskMotion_recovery );
        		}
        	}

			return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* ----- End ---------------------------------------------------------------- */
