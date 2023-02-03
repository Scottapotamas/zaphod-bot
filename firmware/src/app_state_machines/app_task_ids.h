#ifndef APP_TASK_IDS_H
#define APP_TASK_IDS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"

/* -------------------------------------------------------------------------- */

/** The enum AppTaskID contains the identifier for all tasks in the system.
 *  Its order specifies the relative priority of tasks. The scheduler will
 *  always process all events from the highest priority task first and only
 *  move down the priority list when there are no higher priority events to
 *  process.
 *
 *  Event processing is blocking, so while an event is processed it runs to
 *  completion and will block all other tasks (even higher priority ones) from
 *  processing their events, so tasks needs to be carefully designed to
 *  avoid long delays in function calls to lower level libraries.
 */

enum AppTaskID
{
    TASK_IDLE = 0,    // Default system IDLE

    TASK_SUPERVISOR,    // High priority task that oversees the system
    TASK_MOTION,        // Handle the motion command queue and pathing engine, higher level motor supervisor
    TASK_LIGHTING,      // Handle the LED command queue and animations engine
    TASK_EXPANSION,
    TASK_SHUTTER,
                      //	TASK_EXPANSION,			//Handle the internal and external IO

    TASK_MAX,    // Last entry used to define the size of task table
};

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_IDS_H */
