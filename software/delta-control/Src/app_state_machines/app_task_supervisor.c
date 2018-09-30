/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <event_subscribe.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"
#include "qassert.h"
#include "app_config.h"
#include "app_times.h"
#include "app_signals.h"
#include "app_events.h"
#include "app_version.h"

#include "app_task_supervisor.h"
#include "app_task_supervisor_private.h"

#include "status.h"
#include "buzzer.h"
#include "sensors.h"
#include "path_interpolator.h"
#include "configuration.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskSupervisorCreate( AppTaskSupervisor *me,
                         StateEvent        *eventQueueData[ ],
                         const uint8_t     eventQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof(AppTaskSupervisor) );

    // Initialise State Machine State
    AppTaskSupervisorConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask*)me,
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
                                        const StateEvent *e __attribute__((__unused__)) )
{
	button_init( BUTTON_0, AppTaskSupervisorButtonEvent );
	button_init( BUTTON_1, AppTaskSupervisorButtonEvent );

    // Detect user activities
    eventSubscribe( (StateTask*)me, BUTTON_NORMAL_SIGNAL );
    eventSubscribe( (StateTask*)me, MECHANISM_START );
    eventSubscribe( (StateTask*)me, MECHANISM_STOP );
    eventSubscribe( (StateTask*)me, MECHANISM_HOME );

    // motion handler events
    eventSubscribe( (StateTask*)me, MOTION_ERROR );
    eventSubscribe( (StateTask*)me, MOTION_HOMED );
    eventSubscribe( (StateTask*)me, MOTION_DISABLED );

    STATE_INIT( &AppTaskSupervisor_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskSupervisor_main( AppTaskSupervisor *me,
                                      const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        {
        	config_set_main_state(1);
        	//start the board hardware sensors
        	sensors_enable();


            status_green(false);
            status_yellow(false);

        	return 0;
        }

        case BUTTON_NORMAL_SIGNAL:
		   switch( ((ButtonPressedEvent*)e)->id )
		   {
			   case BUTTON_0:
				   eventPublish( EVENT_NEW( StateEvent, MECHANISM_START ) );
				   return 0;

			   case BUTTON_1:
				   eventPublish( EVENT_NEW( StateEvent, MECHANISM_STOP ) );
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
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(2);
            status_green(false);
            status_yellow(false);

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
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(3);
            status_yellow(true);

        	//request a motion handler homing process
            eventPublish( EVENT_NEW( StateEvent, MOTION_PREPARE ) );

        	// timeout incase the motion handler doesn't signal back
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( 8000 ) );
        	//todo use declared servo failure time here
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

		case STATE_EXIT_SIGNAL:
			eventTimerStopIfActive(&me->timer1);
			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Handle startup errors

PRIVATE STATE AppTaskSupervisor_arm_error( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(4);

        	//cleanup and prepare for recovery

        	//send message to UI

        	//blink some lights?

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
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	// send message to UI
        	config_set_main_state(5);
            status_yellow(false);
            status_green(true);

        	//start additional subsystems

        	STATE_TRAN( AppTaskSupervisor_armed );

        	return 0;

        case MOTION_ERROR:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

        case MECHANISM_STOP:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Running Supervisor State

PRIVATE STATE AppTaskSupervisor_armed( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(6);

        	//set up any recurring monitoring processes

        	return 0;

        case MECHANISM_STOP:
        	STATE_TRAN( AppTaskSupervisor_disarm_graceful );
        	return 0;

        case MECHANISM_HOME:
        	{
				// get global position
				CartesianPoint_t position = path_interpolator_get_global_position();

	        	//request a move to 0,0,0
				MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );
				motev->move.type = _POINT_TRANSIT;
				motev->move.ref = _POS_ABSOLUTE;
				motev->move.duration = 1500;
				motev->move.num_pts = 2;

				if( position.x > MM_TO_MICRONS(0.1) && position.y > MM_TO_MICRONS(0.1) && position.z > MM_TO_MICRONS(0.1) )
				{
					motev->move.points[0].x = 0;
					motev->move.points[0].y = 0;
					motev->move.points[0].z = 0;
					motev->move.points[1].x = 0;
					motev->move.points[1].y = 0;
					motev->move.points[1].z = 0;
					eventPublish( (StateEvent*)motev );
				}

				return 0;
			}

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

// Start a graceful shutdown process

PRIVATE STATE AppTaskSupervisor_disarm_graceful( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	config_set_main_state(7);

        	//empty out the motion queue
            eventPublish( EVENT_NEW( StateEvent, MOTION_CLEAR_QUEUE ) );

            stateTaskPostReservedEvent( STATE_STEP1_SIGNAL );

        	//come back and check the position until we are home
        	eventTimerStartEvery( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( 150 ) );

        	return 0;

        case STATE_STEP1_SIGNAL:
        {
        	//request a move to 0,0,0
			MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );

			//transit to starting position
			motev->move.type = _POINT_TRANSIT;
			motev->move.ref = _POS_ABSOLUTE;
			motev->move.duration = 1500;
			motev->move.num_pts = 2;

			motev->move.points[0].x = 0;
			motev->move.points[0].y = 0;
			motev->move.points[0].z = 0;
			motev->move.points[1].x = 0;
			motev->move.points[1].y = 0;
			motev->move.points[1].z = 0;

			eventPublish( (StateEvent*)motev );

            return 0;
        }

        case STATE_TIMEOUT1_SIGNAL:
        {
        	// get global position
        	CartesianPoint_t position = path_interpolator_get_global_position();

        	// Check to make sure the mechanism is at the home position before disabling servo power
        	// Allow a 5 micron error on position in check
        	if( position.x < MM_TO_MICRONS(0.1) && position.y < MM_TO_MICRONS(0.1) && position.z < MM_TO_MICRONS(0.1) && path_interpolator_get_move_done() )
        	{
            	eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );

        	}

        	//todo consider adding a second timeout to e-stop if it doesn't get home in time?

			return 0;
        }

        case MOTION_DISABLED:
        	STATE_TRAN( AppTaskSupervisor_disarmed );
        	return 0;

		case STATE_EXIT_SIGNAL:
			eventTimerStopIfActive(&me->timer1);
			return 0;
    }
    return (STATE)AppTaskSupervisor_main;
}

/* -------------------------------------------------------------------------- */

/*
 * Button handling publishes event
 */

PRIVATE void AppTaskSupervisorButtonEvent( ButtonId_t button,
                                          ButtonPressType_t press_type )
{
	if(press_type == BUTTON_PRESS_TYPE_NORMAL)
	{
	    ButtonEvent *be = EVENT_NEW( ButtonEvent, BUTTON_NORMAL_SIGNAL );
	    if( be )
	    {
	        be->id = button;
	        be->press_type = press_type;
	        eventPublish( (StateEvent*)be );
	    }
	}

}
/* ----- End ---------------------------------------------------------------- */
