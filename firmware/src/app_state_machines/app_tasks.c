/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */
/* Task & State Machine Support */
#include "app_background.h"
#include "app_events.h"
#include "app_hardware.h"
#include "app_signals.h"
#include "app_task_ids.h"
#include "app_times.h"
#include "event_subscribe.h"
#include "global.h"
#include "qassert.h"
#include "state_task.h"
#include "state_tasker.h"

/* Application Tasks */
#include "app_task_led.h"
#include "app_task_motion.h"
#include "app_task_supervisor.h"
#include "app_task_shutter.h"

#include "hal_adc.h"
#include "hal_systick.h"

#include "led_interpolator.h"
#include "path_interpolator.h"
#include "point_follower.h"
#include "effector.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

// ~~~ Event Pool Types ~~~

/** Up to three distinct storage pools. */
EventPool eventPool[3];

/** @note: Select the following typedefs as approximately the largest
 *         within their group of small, medium and large structures.
 *         You need to make sure that EventsLargeType corresponds to the
 *         biggest event that can be allocated.
 */
typedef ButtonPressedEvent EventsSmallType;
typedef ButtonEvent        EventsMediumType;
typedef MotionPlannerEvent EventsLargeType;

// ~~~ Event Pool Storage ~~~
EventsSmallType  eventsSmall[10];     //  __attribute__ ((section (".ccmram")))
EventsMediumType eventsMedium[15];    //  __attribute__ ((section (".ccmram")))
EventsLargeType __attribute__( ( section( ".ccmram" ) ) ) eventsLarge[400];

// ~~~ Event Subscription Data ~~~
EventSubscribers eventSubscriberList[STATE_MAX_SIGNAL];

// ~~~ Task Control Blocks & Event Queues ~~~

AppTaskMotion appTaskMotion;
StateEvent   *appTaskMotionEventQueue[MOVEMENT_QUEUE_DEPTH_MAX];
StateEvent   *appTaskMotionQueue[150];

AppTaskLed  appTaskLed;
StateEvent *appTaskLedEventQueue[LED_QUEUE_DEPTH_MAX];
StateEvent *appTaskLedQueue[250];

AppTaskSupervisor appTaskSupervisor;
StateEvent       *appTaskSupervisorEventQueue[20];

AppTaskShutter appTaskShutter;
StateEvent    *appTaskShutterEventQueue[5];

// ~~~ Tasker ~~~

PRIVATE StateTasker_t mainTasker;
PUBLIC StateTask     *mainTaskTable[TASK_MAX];

/* ----- Public Functions --------------------------------------------------- */

PUBLIC
void app_tasks_init( void )
{
    /* ~~~ Tasker Handling Initialisation ~~~ */
    stateTaskerInit( &mainTasker, mainTaskTable, TASK_MAX );

    /* ~~~ Dynamic Event Pools Initialisation ~~~ */
    eventPoolInit( eventPool,
                   DIM( eventPool ) );

    ALLEGE( eventPoolAddStorage( (StateEvent *)&eventsSmall,
                                 DIM( eventsSmall ),
                                 sizeof( EventsSmallType ) )
            != 0 );

    ALLEGE( eventPoolAddStorage( (StateEvent *)&eventsMedium,
                                 DIM( eventsMedium ),
                                 sizeof( EventsMediumType ) )
            != 0 );

    ALLEGE( eventPoolAddStorage( (StateEvent *)&eventsLarge,
                                 DIM( eventsLarge ),
                                 sizeof( EventsLargeType ) )
            != 0 );

    /* ~~~ Event Subscription Tables Initialisation ~~~ */
    eventSubscribeInit( mainTaskTable, eventSubscriberList, STATE_MAX_SIGNAL );

    /* ~~~ Event Timers Initialisation ~~~ */
    eventTimerInit();

    /* ~~~ Init background processes ~~~ */
    app_background_init();

    /* ~~~ State Machines Initialisation ~~~ */
    StateTask *t;

    // Handle motion controls
    t = appTaskMotionCreate( &appTaskMotion,
                             appTaskMotionEventQueue,
                             DIM( appTaskMotionEventQueue ),
                             appTaskMotionQueue,
                             DIM( appTaskMotionQueue ) );

    stateTaskerAddTask( &mainTasker, t, TASK_MOTION, "Movement" );
    stateTaskerStartTask( &mainTasker, t );

    // Handle LED control
    t = appTaskLedCreate( &appTaskLed,
                          appTaskLedEventQueue,
                          DIM( appTaskLedEventQueue ),
                          appTaskLedQueue,
                          DIM( appTaskLedQueue ) );

    stateTaskerAddTask( &mainTasker, t, TASK_LIGHTING, "Lighting" );
    stateTaskerStartTask( &mainTasker, t );

    // Camera control task
    t = appTaskShutterCreate( &appTaskShutter,
                              appTaskShutterEventQueue,
                              DIM( appTaskShutterEventQueue ) );

    stateTaskerAddTask( &mainTasker, t, TASK_SHUTTER, "ShutterRelease" );
    stateTaskerStartTask( &mainTasker, t );

    // Overseer task
    t = appTaskSupervisorCreate( &appTaskSupervisor,
                                 appTaskSupervisorEventQueue,
                                 DIM( appTaskSupervisorEventQueue ) );

    stateTaskerAddTask( &mainTasker, t, TASK_SUPERVISOR, "Supervisor" );
    stateTaskerStartTask( &mainTasker, t );

    hal_systick_hook( 1, eventTimerTick );
    hal_systick_hook( 2, hal_adc_tick );

    hal_systick_hook( 1, led_interpolator_process );
    hal_systick_hook( 1, path_interpolator_process );
    hal_systick_hook( 1, point_follower_process );
    hal_systick_hook( 1, effector_process );
}

/* -------------------------------------------------------------------------- */

/* Run a cycle of the application state machine dispatcher. Returns true
 * when there is more events to be processed. False when the state machines
 * are idle.
 */

PUBLIC bool
app_tasks_run( void )
{
    /* Run the background processes. */
    app_background();

    /* Run a single task event. */
    return stateTaskerRunEvent( &mainTasker );
}

/* -------------------------------------------------------------------------- */

/** Return a pointer to the task structure identified by id */

PUBLIC StateTask *
app_task_by_id( uint8_t id )
{
    return stateTaskerGetTaskById( &mainTasker, id );
}

/* -------------------------------------------------------------------------- */

/** Clear the task handling statistics */

PUBLIC void
app_task_clear_statistics( void )
{
    stateTaskerClearStatistics( &mainTasker );
}

/* ----- End ---------------------------------------------------------------- */
