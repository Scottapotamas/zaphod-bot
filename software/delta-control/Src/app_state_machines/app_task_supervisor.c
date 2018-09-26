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

            //Todo actually do something in the supervisor state
            status_green(false);
            status_yellow(false);

        	return 0;
        }

        case BUTTON_NORMAL_SIGNAL:
		   switch( ((ButtonPressedEvent*)e)->id )
		   {
			   case BUTTON_0:
			   	   {
				   		eventPublish( EVENT_NEW( StateEvent, MOTION_PREPARE ) );

			   	   }
				   return 0;

			   case BUTTON_1:
			   	   {
			   		   MotionPlannerEvent *motev = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );

			   		   if(motev)
			   		   {
			   			   motev->move.type = _LINE;
			   			   motev->move.ref = _POS_ABSOLUTE;
			   			   motev->move.duration = 650;

			   			   //start
			   			   motev->move.points[0].x = 0;
			   			   motev->move.points[0].y = 0;
			   			   motev->move.points[0].z = 0;

			   			   //dest
			   			   motev->move.points[1].x = 0;
			   			   motev->move.points[1].y = 0;
			   			   motev->move.points[1].z = MM_TO_MICRONS(5);

			   			   motev->move.num_pts = 2;

			   			   eventPublish( (StateEvent*)motev );
			   		   }
				   		eventPublish( EVENT_NEW( StateEvent, EUI_PING ) );
			   	   }
				   return 0;
			   default:
				   break;
		   }
		   break;

        case STATE_INIT_SIGNAL:
            STATE_INIT( &AppTaskSupervisor_arm_start );
            return 0;

    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

// Start a mechanism startup

PRIVATE STATE AppTaskSupervisor_arm_start( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        	//request a motion handler homing process

        	return 0;

        	//catch events for success and failures

		case STATE_EXIT_SIGNAL:

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
        	//cleanup and prepare for recovery

        	//send message to UI

        	//blink some lights?

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

        	//start additional subsystems


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
        	//set up any recurring monitoring processes

        	return 0;

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

        	//empty out the motion queue

        	//request a move to 0,0,0

        	//come back and check the position until we are home


        	return 0;

		case STATE_EXIT_SIGNAL:
			//call a full motor shutdown

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
