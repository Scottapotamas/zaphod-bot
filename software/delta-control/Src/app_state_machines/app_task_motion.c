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
PRIVATE STATE 	AppTaskMotion_idle		( AppTaskMotion *me, const StateEvent *e );
PRIVATE STATE 	AppTaskMotion_run		( AppTaskMotion *me, const StateEvent *e );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskMotionCreate(  AppTaskMotion *me,
					  StateEvent        *eventQueueData[ ],
					  const uint8_t     eventQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof(AppTaskMotion) );

    // Initialise State Machine State
    AppTaskMotionConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask*)me,
                            eventQueueData,
                            eventQueueSize,
                            0,
                            0 );
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
    eventSubscribe( (StateTask*)me, MOTION_HEAD );
    eventSubscribe( (StateTask*)me, MOTION_TAIL );
    eventSubscribe( (StateTask*)me, MOTION_REQUEST );
    eventSubscribe( (StateTask*)me, MECHANISM_HOMED );
    eventSubscribe( (StateTask*)me, MECHANISM_ERROR );

    STATE_INIT( &AppTaskMotion_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_main( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        {

        	return 0;
        }

        case STATE_INIT_SIGNAL:
            STATE_INIT( &AppTaskMotion_home );
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
        	servo_run_startup( _CLEARPATH_1 );
        	servo_run_startup( _CLEARPATH_2 );
        	servo_run_startup( _CLEARPATH_3 );

            eventTimerStartOnce( &me->timer1,
                                 (StateTask* )me,
                                 (StateEvent* )&stateEventReserved[ STATE_TIMEOUT1_SIGNAL ],
                                 MS_TO_TICKS( 5000 ) );

            // todo define homing check timeout elsewhere
        	return 0;

        case STATE_TIMEOUT1_SIGNAL:

        	//todo supervisor to verify that the motors have all completed their driver level homing proceedure

        	STATE_TRAN( AppTaskMotion_idle );

			return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskMotion_main;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_idle( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

        	return 0;

        case MOTION_REQUEST:
        	/*
        	Movement_t mv = ((MotionPlannerEvent*)e)->move;
            switch( mv.type )
            {
                case _POINT_TRANSIT:

                	STATE_TRAN( AppTaskMotion_run );
                	break;

                case _LINE:

                	break;

                case _CATMULL_SPLINE:

                	break;
            }
            */
        	return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskMotion_main;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_run( AppTaskMotion *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

        	//publish that we are starting a movement

        	return 0;

		case STATE_EXIT_SIGNAL:

			//publish that we have finished a movement

			return 0;
    }
    return (STATE)AppTaskMotion_main;
}

/* ----- End ---------------------------------------------------------------- */
