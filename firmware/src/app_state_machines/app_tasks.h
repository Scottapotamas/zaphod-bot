#ifndef APP_TASKS_H
#define APP_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "state_task.h"

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
app_tasks_init( void );

/* -------------------------------------------------------------------------- */

/* Run a cycle of the application state machine dispatcher. Returns true
 * when there is more events to be processed. False when the state machines
 * are idle.
 */

PUBLIC bool
app_tasks_run( void );

/* -------------------------------------------------------------------------- */

/** Return a pointer to the task structure identified by id */

PUBLIC StateTask *
app_task_by_id( uint8_t id );

/* -------------------------------------------------------------------------- */

/** Clear the task handling statistics */

PUBLIC void
app_task_clear_statistics( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASKS_H */
