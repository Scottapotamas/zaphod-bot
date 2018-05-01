/**
 * @file      state_tasker.h
 *
 * @brief     The tasker is responsible for executing the events for
 *            state tasks and managing priorities.
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2017 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef STATE_TASKER_H
#define STATE_TASKER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "state_task.h"

/* ----- Defines ------------------------------------------------------------ */

#ifndef STATE_TASKER_MAX_TASKS
  #define STATE_TASKER_MAX_TASKS      16     //!< Maximum number of tasks supported
#endif

/* ----- Types -------------------------------------------------------------- */

typedef struct
{
    uint8_t                max_tasks;       /** maximum number of tasks */
    StateTask              *current;        /** current running task */
    StateTask              *previous;       /** previous running task */
    StateTask              **tasks;         /** Table of tasks pointers */
} StateTasker_t;

/* -------------------------------------------------------------------------- */

/** Init the initial tasker data structures
  */

PUBLIC void
stateTaskerInit( StateTasker_t *me, StateTask **tasks, uint8_t num_tasks );

/* -------------------------------------------------------------------------- */

/** Add a new task to the tasker structure.
 *  Task need to been created first.
 */

PUBLIC void
stateTaskerAddTask( StateTasker_t *me,
                    StateTask     *task,
                    const uint8_t priority,
                    const char    * name );

/* -------------------------------------------------------------------------- */

/** Add a new task to the tasker structure.
 *  Task need to been created first.
 */

PUBLIC void
stateTaskerStartTask( StateTasker_t * me, StateTask *task );

/* -------------------------------------------------------------------------- */

/** Run a single state machine event. Returns TRUE when there are more events
 *  in the task queues. See also the task-priority module which has
 *  a callback function that can be used to reactivate a scheduler when event
 *  handling is temporarily halted (e.g. when taskRunEvent returned FALSE).
 */

PUBLIC bool
stateTaskerRunEvent( StateTasker_t * me );

/* -------------------------------------------------------------------------- */

/** Return the name of the task with the indicated ID.
 */

PUBLIC const char *
stateTaskerGetTaskName( StateTasker_t * me, uint8_t task_id );

/* -------------------------------------------------------------------------- */

PUBLIC StateTask *
stateTaskerGetTaskById( StateTasker_t *me, uint8_t id );

/* -------------------------------------------------------------------------- */

PUBLIC void
stateTaskerClearStatistics( StateTasker_t *me );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* STATE_TASKER_H */
