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

#include "hal_uuid.h"
#include "status.h"

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

        	return 0;
        }

        case BUTTON_NORMAL_SIGNAL:
		   switch( ((ButtonPressedEvent*)e)->id )
		   {
			   case BUTTON_0:
                   //eventPublish( EVENT_NEW( StateEvent, MOTION_REQUEST ) );

				   /*
		        	MotionPlannerEvent * motev = EVENT_NEW( MotionPlannerEvent, MOTION_REQUEST );

			        motev->move.type = _POINT_TRANSIT;
			        motev->move.type = _POS_ABSOLUTE;
			        motev->move.duration = 650;

			        CartesianPoint_t line[] = {
			        		{0, 0, 0},
							{50, 50, 50}
			        };

			        motev->move.points = line;
			        motev->move.num_pts = 2;

		            eventPublish( (StateEvent*)motev );
				    */
				   return 0;

			   default:
				   break;
		   }
		   break;

        case STATE_INIT_SIGNAL:
            STATE_INIT( &AppTaskSupervisor_load_config );
            return 0;

    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskSupervisor_load_config( AppTaskSupervisor *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

        	return 0;

        case STATE_TIMEOUT1_SIGNAL:

			return 0;

		case STATE_EXIT_SIGNAL:

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
