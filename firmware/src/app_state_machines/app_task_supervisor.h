#ifndef APP_TASK_SUPERVISOR_H
#define APP_TASK_SUPERVISOR_H

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

typedef enum
{
    SUPERVISOR_NONE = 0,
    SUPERVISOR_IDLE,
    SUPERVISOR_ARMING,
    SUPERVISOR_ERROR,
    SUPERVISOR_ARMED,
    SUPERVISOR_DISARMING,
    SUPERVISOR_INVALID,
} SupervisorStates_t;

/* ----- State Task Control Block ------------------------------------------- */

/** Application Task Control Block File System */
typedef struct AppTaskSupervisor AppTaskSupervisor;
struct AppTaskSupervisor
{
    // ~~~ Core task structure ~~~
    StateTask super;    ///< Core task structure

    // ~~~ Task Timers ~~~
    EventTimer timer1;    ///< Local state timer 1
    EventTimer timer2;    ///< Local state timer 2

    // ~~~ Task Variables ~~~
    SupervisorStates_t armed;
    uint8_t active_control_mode;    // mode of end effector control
    uint8_t requested_control_mode;    // mode of end effector control

    uint8_t requested_demo_program;    // Demonstration sub-program to run
};

/* ----- Public Functions --------------------------------------------------- */

/**
 * @brief  Create the supervisor I/O task
 * @param  me             Reference to own task structure
 * @param  priority       Task priority level (fixed at compile time).
 * @param  eventQueueData event queue for this task
 * @param  eventQueueSize size of queue
 * @return StateTask *
 */

PUBLIC StateTask *
appTaskSupervisorCreate( AppTaskSupervisor *me,
                         StateEvent        *eventQueueData[],
                         const uint8_t      eventQueueSize );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_SUPERVISOR_H */
