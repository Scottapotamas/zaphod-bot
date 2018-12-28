/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <event_subscribe.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_times.h"
#include "app_signals.h"
#include "app_events.h"

#include "app_task_expansion.h"

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskExpansionConstructor( AppTaskExpansion *me );

PRIVATE void AppTaskExpansion_initial( AppTaskExpansion *me, const StateEvent *e );

PRIVATE STATE AppTaskExpansion_main( AppTaskExpansion *me, const StateEvent *e );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskExpansionCreate(	AppTaskExpansion *me,
                        StateEvent        *eventQueueData[],
						const uint8_t     eventQueueSize,
						const ExpansionInstance_t instance )
{
    // Clear all task data
    memset( me, 0, sizeof(AppTaskExpansion) );

    // Initialise State Machine State
    AppTaskExpansionConstructor( me );

    me->instance = instance;

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask*)me,
                            eventQueueData,
                            eventQueueSize,
                            0,
                            0 );
}

/* ----- Private Functions -------------------------------------------------- */

// State Machine Construction
PRIVATE void AppTaskExpansionConstructor( AppTaskExpansion *me )
{
    stateTaskCtor( &me->super, (State)&AppTaskExpansion_initial );
}

/* -------------------------------------------------------------------------- */

// State Machine Initial State
PRIVATE void AppTaskExpansion_initial( AppTaskExpansion *me,
                                        const StateEvent *e __attribute__((__unused__)) )
{

    STATE_INIT( &AppTaskExpansion_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskExpansion_main( AppTaskExpansion *me,
                                     const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
        {

        	return 0;
        }

        case STATE_INIT_SIGNAL:
            //STATE_INIT( &AppTaskCommunication_something );
            return 0;

    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */



/* ----- End ---------------------------------------------------------------- */
