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

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void 	AppTaskMotionConstructor( AppTaskMotion *me );

PRIVATE void 	AppTaskMotion_initial	( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_main		( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_home		( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_inactive	( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_active	( AppTaskMotion *me, const StateEvent *e );

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
    eventSubscribe( (StateTask*)me, MOTION_STOP );

    eventSubscribe( (StateTask*)me, MOTION_REQUEST );
    eventSubscribe( (StateTask*)me, MOTION_HEAD );
    eventSubscribe( (StateTask*)me, MOTION_TAIL );

    STATE_INIT( &AppTaskMotion_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_main( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	//disable servos
        	servo_stop( _CLEARPATH_1 );
        	servo_stop( _CLEARPATH_2 );
        	servo_stop( _CLEARPATH_3 );
        #ifdef EXPANSION_SERVO
        	servo_stop( _CLEARPATH_4 );
        #endif

        	return 0;

        case MOTION_PREPARE:
        	STATE_TRAN( AppTaskMotion_home );

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

        	//reset the motors and let them home
        	servo_start( _CLEARPATH_1 );
        	servo_start( _CLEARPATH_2 );
        	servo_start( _CLEARPATH_3 );
#ifdef EXPANSION_SERVO
        	servo_start( _CLEARPATH_4 );
#endif

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
                eventPublish( EVENT_NEW( StateEvent, MECHANISM_HOMED ) );
            	STATE_TRAN( AppTaskMotion_inactive );
        	}
        	else
        	{
        		//allow subsequent homing check retries
        		if( me->retries++ > SERVO_HOMING_SUPERVISOR_RETRIES )
        		{
        			eventTimerStopIfActive(&me->timer1);
        			eventPublish( EVENT_NEW( StateEvent, MECHANISM_ERROR ) );
                	// todo work out what to do when they failed to home correctly

        		}
        	}

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

            // Check for queued events
            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	return 0;

        case STATE_STEP1_SIGNAL:
        {
            // Check the queue for pending movements
        	MotionPlannerEvent * next = (MotionPlannerEvent*)eventQueueGet( &me->super.requestQueue );

        	if( next )
            {
                /* Setup next movement */
                me->mpe = *next;
                eventPoolGarbageCollect( (StateEvent*)next );
                STATE_TRAN( AppTaskMotion_active );
            }
            return 0;
        }

        case MOTION_REQUEST:
			{
				//want to do a movement, process immediately without the queue
				MotionPlannerEvent *ape = (MotionPlannerEvent*)e;
				me->mpe = *ape; /* Save event */
				STATE_TRAN( AppTaskMotion_active );
			}
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
			{
				Movement_t * current_move = &me->mpe.move;

				//send the current movement pointer to the pathing engine
				path_interpolator_set_objective( current_move );

				//we expect the move to be completed within the specified duration
				//check back with the pathing engine when that duration is up
				eventTimerStartOnce( &me->timer1,
									 (StateTask* )me,
									 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
									 current_move->duration + 1 );
			}
			return 0;

        case STATE_TIMEOUT1_SIGNAL:

        	if( path_interpolator_get_progress() > 0.999 )
        	{
            	STATE_TRAN( AppTaskMotion_inactive );
        	}

        	return 0;

        case MOTION_REQUEST:
        	{
        		//already in motion, so add this one to the queue
				MotionPlannerEvent *mpe = (MotionPlannerEvent*)e;

				// Add the movement request to the queue if we have room
				if( eventQueueUsed( &me->super.requestQueue ) < 10 )	//10 queue size
				{
					if( mpe->move.duration)
					{
						eventQueuePutFIFO( &me->super.requestQueue, (StateEvent*)e );
					}
				}
				else
				{
					//queue full
				}
        	}
        	return 0;

        case MOTION_STOP:
        	{
				// Empty the queue
				StateEvent * next = eventQueueGet( &me->super.requestQueue );
				while( next )
				{
					eventPoolGarbageCollect( (StateEvent*)next );
					next = eventQueueGet( &me->super.requestQueue );
				}

            	STATE_TRAN( AppTaskMotion_inactive );
        	}
        	return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* ----- End ---------------------------------------------------------------- */
