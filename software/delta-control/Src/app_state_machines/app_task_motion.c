/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <event_subscribe.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_times.h"
#include "app_signals.h"
#include "app_events.h"

#include "app_task_motion.h"

#include "clearpath.h"
#include "kinematics.h"

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskMotionConstructor( AppTaskMotion *me );

PRIVATE void AppTaskMotion_initial( AppTaskMotion *me,
                                    const StateEvent *e );

PRIVATE STATE AppTaskMotion_main( AppTaskMotion *me,
                                  const StateEvent *e );

PRIVATE STATE AppTaskMotion_home( AppTaskMotion *me,
                                        const StateEvent *e );

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
PRIVATE void AppTaskMotion_initial( AppTaskMotion *me,
                                        const StateEvent *e __attribute__((__unused__)) )
{

    STATE_INIT( &AppTaskMotion_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskMotion_main( AppTaskMotion *me,
                                      const StateEvent *e )
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

PRIVATE STATE AppTaskMotion_home( AppTaskMotion *me,
                                         	 const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

        	//reset the motors and let them home
        	servo_run_startup( _CLEARPATH_1 );
        	servo_run_startup( _CLEARPATH_2 );
        	servo_run_startup( _CLEARPATH_3 );

        	return 0;

        case STATE_TIMEOUT1_SIGNAL:

			return 0;

        case STATE_TIMEOUT2_SIGNAL:

			return 0;

		case STATE_EXIT_SIGNAL:

			return 0;
    }
    return (STATE)AppTaskMotion_main;
}

/* ----- End ---------------------------------------------------------------- */
