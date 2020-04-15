#ifndef APP_TASK_LED_H
#define APP_TASK_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "event_timer.h"
#include "global.h"
#include "state_task.h"

/* ----- State Task Control Block ------------------------------------------- */

/** Application Task Control Block File System */
typedef struct AppTaskLed AppTaskLed;

struct AppTaskLed
{
    // ~~~ Core task structure ~~~
    StateTask super;

    // ~~~ Task Timers ~~~
    EventTimer timer1;
    EventTimer timer2;

    // ~~~ Task Variables ~~~
    uint16_t identifier_to_execute;
};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskLedCreate( AppTaskLed *  me,
                  StateEvent *  eventQueueData[],
                  const uint8_t eventQueueSize,
                  StateEvent *  lightingQueue[],
                  const uint8_t lightingQueueSize );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_LED_H */
