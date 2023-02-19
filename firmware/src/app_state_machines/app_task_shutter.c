/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "app_config.h"
#include "app_events.h"
#include "app_signals.h"
#include "app_times.h"
#include "event_subscribe.h"
#include "qassert.h"

#include "app_task_shutter.h"
#include "shutter_release.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Function Definitions --------------------------------------- */

PRIVATE void AppTaskShutterConstructor( AppTaskShutter *me );

PRIVATE void  AppTaskShutter_initial( AppTaskShutter *me, const StateEvent *e );
PRIVATE STATE AppTaskShutter_main( AppTaskShutter *me, const StateEvent *e );
PRIVATE STATE AppTaskShutter_capture( AppTaskShutter *me, const StateEvent *e );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskShutterCreate( AppTaskShutter *me,
                      StateEvent    *eventQueueData[],
                      const uint8_t  eventQueueSize )
{
    // Clear all task data
    memset( me, 0, sizeof( AppTaskShutter ) );

    // Initialise State Machine State
    AppTaskShutterConstructor( me );

    /* Initialise State Machine Task */
    return stateTaskCreate( (StateTask *)me,
                            eventQueueData,
                            eventQueueSize,
                            0,
                            0 );
}

/* ----- Private Functions -------------------------------------------------- */

// State Machine Construction
PRIVATE void AppTaskShutterConstructor( AppTaskShutter *me )
{
    stateTaskCtor( &me->super, (State)&AppTaskShutter_initial );
}

/* -------------------------------------------------------------------------- */

// State Machine Initial State
PRIVATE void AppTaskShutter_initial( AppTaskShutter *me, const StateEvent *e __attribute__( ( __unused__ ) ) )
{
    eventSubscribe( (StateTask *)me, CAMERA_CAPTURE );

    shutter_init();

    STATE_INIT( &AppTaskShutter_main );
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskShutter_main( AppTaskShutter *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:

            return 0;

        case CAMERA_CAPTURE: {
            CameraShutterEvent *trigger = (CameraShutterEvent *)e;

            // Ignore 0 duration captures
            if( trigger->exposure_time )
            {
                eventTimerStartOnce( &me->timer1,
                                      (StateTask *)me,
                                      (StateEvent *)&stateEventReserved[STATE_TIMEOUT1_SIGNAL],
                                      MS_TO_TICKS( trigger->exposure_time ) );
                STATE_TRAN( AppTaskShutter_capture );
            }

            return 0;
        }
    }
    return (STATE)hsmTop;
}

/* -------------------------------------------------------------------------- */

PRIVATE STATE AppTaskShutter_capture( AppTaskShutter *me, const StateEvent *e )
{
    switch( e->signal )
    {
        case STATE_ENTRY_SIGNAL:
            shutter_capture( true );
            return 0;

        case STATE_TIMEOUT1_SIGNAL:
            shutter_capture( false );
            STATE_TRAN( AppTaskShutter_main );
            return 0;

        case CAMERA_CAPTURE:
            // Shutter command during an existing capture cancels the running capture
            shutter_capture( false );
            STATE_TRAN( AppTaskShutter_main );
            return 0;

        case STATE_EXIT_SIGNAL:
            eventTimerStopIfActive( &me->timer1 );
            return 0;
    }
    return (STATE)hsmTop;
}

/* ----- End ---------------------------------------------------------------- */
