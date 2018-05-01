/**
 * @file    task.c
 *
 * @brief   Tasking Kernel
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "state_task.h"
#include "qassert.h"
#include "state_event.h"

/* -------------------------------------------------------------------------- */

//DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Private Functions -------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
stateTaskCtor( StateTask * restrict me,
               State     initialState )
{
    hsmCtor((Hsm*)me, (PseudoState)initialState);
}

/* -------------------------------------------------------------------------- */

//! Create the task control structure
PUBLIC StateTask *
stateTaskCreate( StateTask    * restrict me,
                 StateEvent   * restrict eventQueueData[],
                 uint8_t      eventQueueSize,
                 StateEvent   * restrict requestQueueData[],
                 uint8_t      requestQueueSize )
{
    // Add the event queues.
    eventQueueInit( &me->eventQueue,   eventQueueData,   eventQueueSize );
    eventQueueInit( &me->requestQueue, requestQueueData, requestQueueSize );

    return me;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
stateTaskPostFIFO( StateTask *t, const StateEvent *e )
{
    if( e )
    {
        if( t )
        {
            if( eventQueuePutFIFO( &t->eventQueue, (StateEvent*)e ) )
            {
                t->ready = true;
                return true;
            }
        }
    }
    return false;    // Failed to queue the event (also for a NULL event).
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
stateTaskPostLIFO( StateTask *t, const StateEvent *e )
{
    if( e )
    {
        if( t )
        {
            if( eventQueuePutLIFO( &t->eventQueue, (StateEvent*)e ) )
            {
                t->ready = true;
                return true;
            }
        }
    }
    return false;    // Failed to queue the event (also for a NULL event).
}

/* ----- End ---------------------------------------------------------------- */
