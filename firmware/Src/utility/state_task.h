/**
 * @file      state_task.h
 *
 * @brief     Tasking Kernel
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef STATE_TASK_H
#define STATE_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "state_hsm.h"
#include "state_event.h"
#include "event_queue.h"
#include "event_pool.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types -------------------------------------------------------------- */

/** StateTask Control Block */

typedef struct
{
    Hsm           super;                /** State machine super structure */
    uint8_t       id;
    const char  * name;
    bool          ready;                /** true when ready to run */
    uint32_t      burst;
    uint32_t      waiting;
    uint32_t      burst_max;
    uint32_t      waiting_max;
    void *        tasker;
    EventQueue    eventQueue;
    EventQueue    requestQueue;
} StateTask;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~        Event Queue Functions                                          ~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//! Queue events for the tasks processing. Returns true when the queuing was
/// successful and false when the event failed to be queued (which should be
/// a fatal error but is a return status here to allow checking at higher
/// levels so it is easier to debug which queue and where the problem is).
PUBLIC bool
stateTaskPostFIFO( StateTask *t, const StateEvent *e );

PUBLIC bool
stateTaskPostLIFO( StateTask *t, const StateEvent *e );

//! Flush/discard all request on the request queue
PUBLIC void
stateTaskFlushRequests( StateTask *t );

// Convenience macro post for example a STEP_SIGNAL event to ourselves.
#define stateTaskPostReservedEvent( _event_ ) \
            stateTaskPostFIFO( ((StateTask*)me), (StateEvent*)&stateEventReserved[_event_]);

/* -------------------------------------------------------------------------- */

//! Initialise the core state machine internal data
/// Needs to be called from the task constructor after tasks specific
/// attributes are initialised.
PUBLIC void
stateTaskCtor( StateTask * restrict me, State initialState );

/* -------------------------------------------------------------------------- */

//! Create the internal task structure and its event queues
PUBLIC StateTask *
stateTaskCreate( StateTask    * restrict me,
                 StateEvent   * restrict eventQueueData[],
                 uint8_t      eventQueueSize,
                 StateEvent   * restrict requestQueueData[],
                 uint8_t      requestQueueSize );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* STATE_TASK_H */
