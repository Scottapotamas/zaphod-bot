/**
 * @file    state_tasker.c
 *
 * @brief   Tasking Kernel
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2017 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "state_tasker.h"
#include "qassert.h"
#include "event_queue.h"
#include "state_event.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */


/* ----- Private Prototypes ------------------------------------------------- */

PRIVATE void
stateTaskerBumpWaiting( StateTasker_t * me );

PRIVATE StateTask  *
stateTaskerNext( StateTasker_t * me );

/* ----- Public Functions --------------------------------------------------- */

/** Init the initial tasker data structures
  */

PUBLIC void
stateTaskerInit( StateTasker_t * me, StateTask **tasks, uint8_t num_tasks )
{
    memset( me, 0, sizeof( StateTasker_t ) );
    me->max_tasks = num_tasks;
    me->tasks = tasks;
}

/* -------------------------------------------------------------------------- */

/** Add a new task to the tasker structure.
 *  Task need to been created first.
 */

PUBLIC void
stateTaskerAddTask( StateTasker_t *me,
                    StateTask     *task,
                    const uint8_t priority,
                    const char    *name )
{
    REQUIRE( 0 < priority );              /* Higher than idle */
    REQUIRE( priority < me->max_tasks );  /* Less than num_task */
    REQUIRE( me->tasks[priority] == (StateTask*)0 ); /* Not init yet */

    task->id          = priority;
    task->ready       = false;
    task->name        = name;
    task->burst       = 0;
    task->waiting     = 0;
    task->burst_max   = 0;
    task->waiting_max = 0;

    task->tasker      = me;     /* Keep reference to tasker */

    me->tasks[priority] = task;
}

/* -------------------------------------------------------------------------- */

/** Add a new task to the tasker structure.
 *  Task need to been created first.
 */

PUBLIC void
stateTaskerStartTask( StateTasker_t * me __attribute__((__unused__)),
                      StateTask     * task )
{
    // Run the initial transition.
    hsmInit( (Hsm*)task, (StateEvent*)0 );
}

/* -------------------------------------------------------------------------- */

/** Run a single state machine event. Returns TRUE when there are more events
 *  in the task queues. See also the task-priority module which has
 *  a callback function that can be used to reactivate a scheduler when event
 *  handling is temporarily halted (e.g. when taskRunEvent returned FALSE).
 */

PUBLIC bool
stateTaskerRunEvent( StateTasker_t * me )
{
    stateTaskerBumpWaiting( me );
    me->current = stateTaskerNext( me );
    if( me->current )
    {
        StateEvent *e;

        {
            CRITICAL_SECTION_VAR();
            CRITICAL_SECTION_START();
            e = eventQueueGet( &me->current->eventQueue );
            CRITICAL_SECTION_END();
        }
        me->current->waiting = 0;
        if( me->previous == me->current )
        {
            me->current->burst++;
            if( me->current->burst > me->current->burst_max )
            {
                me->current->burst_max = me->current->burst;
            }
        }
        hsmDispatch( (Hsm*)me->current, e );
        eventPoolGarbageCollect( e );

        {
            CRITICAL_SECTION_VAR();
            CRITICAL_SECTION_START();
            if( eventQueueUsed( &me->current->eventQueue ) == 0 )
            {
                me->current->burst = 0;
                me->current->ready = false;
            }
            CRITICAL_SECTION_END();
        }
    }

    me->previous = me->current;
    return stateTaskerNext( me ) != NULL;
}

/* -------------------------------------------------------------------------- */

/** Return the name of the task with the indicated ID.
 */

PUBLIC const char *
stateTaskerGetTaskName( StateTasker_t * me, uint8_t task_id )
{
    if( ( task_id < me->max_tasks ) && ( me->tasks[task_id] ) )
    {
        return me->tasks[task_id]->name;
    }
    return "";
}

/* -------------------------------------------------------------------------- */

PUBLIC StateTask *
stateTaskerGetTaskById( StateTasker_t *me, uint8_t id )
{
    if( id < me->max_tasks )
    {
        return me->tasks[id];
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
stateTaskerClearStatistics( StateTasker_t *me )
{
    for( uint8_t id = me->max_tasks-1; id > 0; id-- )
    {
        if( ( me->tasks[id] ) )
        {
            me->tasks[id]->burst_max = 0;
            me->tasks[id]->waiting_max = 0;
        }
    }
}

/* ----- Private Functions -------------------------------------------------- */

/** Bump the waiting counter for any ready to run tasks */

PRIVATE void
stateTaskerBumpWaiting( StateTasker_t * me )
{
    for( uint8_t id = me->max_tasks-1; id > 0; id-- )
    {
        if( ( me->tasks[id] ) && ( me->tasks[id]->ready ) )
        {
            me->tasks[id]->waiting++;
            if( me->tasks[id]->waiting > me->tasks[id]->waiting_max )
            {
                me->tasks[id]->waiting_max = me->tasks[id]->waiting;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */

/** Return the task * that is the next to run */

PRIVATE StateTask  *
stateTaskerNext( StateTasker_t * me )
{
    for( uint8_t id = me->max_tasks-1; id > 0; id-- )
    {
        if( ( me->tasks[id] ) && ( me->tasks[id]->ready ) )
        {
            return me->tasks[id];
        }
    }
    return NULL;
}

/* ----- End ---------------------------------------------------------------- */
