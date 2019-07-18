#ifndef APP_TASK_MOTION_H
#define APP_TASK_MOTION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"
#include "state_task.h"
#include "event_timer.h"

/* ----- State Task Control Block ------------------------------------------- */

/** Application Task Control Block File System */
typedef struct AppTaskMotion AppTaskMotion;

struct AppTaskMotion
{
    // ~~~ Core task structure ~~~
    StateTask		super;

    // ~~~ Task Timers ~~~
    EventTimer		timer1;
    EventTimer		timer2;

    // ~~~ Task Variables ~~~
    uint8_t 		counter;
    uint8_t			retries;

    uint16_t identifier_to_execute;

    MotionPlannerEvent mpe;	//current movement to execute

};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskMotionCreate( AppTaskMotion *me,
					 StateEvent    *eventQueueData[ ],
					 const uint8_t eventQueueSize,
					 StateEvent    *movementQueue[ ],
					 const uint8_t movementQueueSize );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_MOTION_H */
