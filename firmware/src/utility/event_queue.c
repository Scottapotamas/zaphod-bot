/**
 * @file    event_queue.c
 *
 * @brief   The Event Queue support services support the initialisation,
 *          getting and putting events into a circular queue.
 *
 * @note    The Event Queue's are really queues with pointers to events!
 *          Events need to be allocated from the event-pool, or statically
 *          allocated (like the standard events in event.c
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "qassert.h"
#include "event_queue.h"
#include "state_task.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC EventQueue *eventQueueInit( EventQueue * restrict me,
                                   StateEvent * restrict queueStorage[],
                                   uint8_t    num_entries )
{
    REQUIRE(me);

    if( (queueStorage != 0) && (num_entries > 0) )
    {
        // Remember where the queue storage is
        me->entries    = queueStorage;
        me->front      = NULL; // no active event in the queue

        // Constants to remember the size of the queue
        me->size       = num_entries;

        // Active content indexes
        me->head       = 0;
        me->tail       = 0;

        // Counters to keep track of usage
        me->used       = 0;
        me->max        = 0;
    }
    else
    {
        // No queue
        me->entries    = NULL;
        me->front      = NULL;
        me->size       = 0;
        me->head       = 0;
        me->tail       = 0;
        me->used       = 0;
        me->max        = 0;
    }
    return me;
}

/* -------------------------------------------------------------------------- */

//! Return the number events in the queue. 0 when none. The frontEvt
/// pointer keeps an extra event just outside the queue so we have to
/// adjust the nUsed count for this.
PUBLIC uint8_t
eventQueueUsed( EventQueue * restrict queue )
{
    if( queue && queue->front )
    {
        return queue->used + 1;
    }
    else
    {
        return 0;
    }
}

/* -------------------------------------------------------------------------- */

//! Retrieve an event from the queue or NULL when it is empty
PUBLIC StateEvent *
eventQueueGet( EventQueue * restrict queue )
{
    register StateEvent *e;

    // Must actually have queue storage
    REQUIRE( (queue->entries) && (queue->size > 0) );

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    // Get the front event
    e = queue->front;

    // Update the house keeping
    if( queue->used > 0 ) // something in the queue?
    {
        --queue->used;                   // one less event in the ring-buffer
        queue->front = queue->entries[queue->tail]; // remove from the tail
        if( ++queue->tail >= queue->size )
        {
            queue->tail = 0;
        }
    }
    else
    {
        // The front event was the only one in the queue, so now we
        // are empty. Next time we get called we return NULL.
        queue->front = NULL;
    }

    CRITICAL_SECTION_END();

    return e;
}

/* -------------------------------------------------------------------------- */

//! Look at the head of the queue
PUBLIC StateEvent *
eventQueuePeek( EventQueue * restrict queue )
{
    // Simply return the front event in the queue
    return queue->front;
}

/* -------------------------------------------------------------------------- */

//! Deposit an event in the queue and return true when this was successful
/// returns false when the queue was full. The frontEvt pointer keeps a
/// shortcut to the most front event which speeds up access when there
/// aren't really any events in the queue anyway. When the event is failed
/// to be queued, the caller needs to ensure that the event is properly
/// deallocated.
PUBLIC bool
eventQueuePutFIFO( EventQueue * restrict queue, StateEvent * restrict e )
{
    bool eventQueued = true; // Assume Success

    REQUIRE( queue );
    REQUIRE( (queue->entries) && (queue->size > 0) );
    REQUIRE( e );

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    // If this is a dynamic allocated event,
    // update the reference count for this event
    if( e->dynamic.poolId != 0 )
    {
        e->dynamic.useCount++;
    }

    // Insert into the queue
    if( queue->front == NULL )  // is the queue empty?
    {
        queue->front = e;    // deliver event directly
    }
    else // queue is not empty, leave event in the ring-buffer
    {
        // Is there room?
        if( queue->used < queue->size )
        {
            // Update the number of events and remember maximum
            if( ++queue->used > queue->max )
            {
                queue->max = queue->used;
            }

            // Insert event at the head
            queue->entries[queue->head] = e;

            // Wrap the head as required
            if( ++queue->head >= queue->size )
            {
                queue->head = 0;
            }
        }
        else // queue is full, return failure
        {
            // If this is a dynamic allocated event,
            // undo the reference count increase we did earlier for this event
            if( e->dynamic.poolId != 0 )
            {
                e->dynamic.useCount--;
            }
            eventQueued = false;
        }
    }
    CRITICAL_SECTION_END();
    return eventQueued;
}

/* -------------------------------------------------------------------------- */

//! Deposit an event in the queue and return true when this was successful
/// returns false when the queue was full. When the event is failed
/// to be queued, the caller needs to ensure that the event is properly
/// deallocated.
PUBLIC bool
eventQueuePutLIFO( EventQueue * restrict queue, StateEvent * restrict e )
{
    bool eventQueued = true; // Assume Success
    REQUIRE( queue );
    REQUIRE( e );

    // Must actually have queue storage
    REQUIRE(queue->entries);

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    // If this is a dynamic allocated event,
    // update the reference count for this event
    if(e->dynamic.poolId != 0)
        e->dynamic.useCount++;

    // Insert into the queue
    if( queue->front == NULL ) // is the queue empty?
    {
        queue->front = e;     // deliver event directly
    }
    else // queue is not empty, leave event in the ring-buffer
    {
        // Is there room?
        if( queue->used < queue->size )
        {
            // Update the number of events and remember maximum
            if (++queue->used > queue->max)
            {
                queue->max = queue->used;
            }

            // make room at the tail of the queue
            if( queue->tail > 0 )
            {
                queue->tail--;
            }
            else
            {
                queue->tail = queue->size - 1;
            }

            // push the current front event back into the queue
            queue->entries[queue->tail] = queue->front;

            // assign the new event to the front
            queue->front = e;
        }
        else // queue is full, return failure
        {
            // If this is a dynamic allocated event,
            // decrement the reference count for this event
            if( e->dynamic.poolId != 0 )
            {
                e->dynamic.useCount--;
            }
            eventQueued = false;
        }
    }
    CRITICAL_SECTION_END();
    return eventQueued;
}

/* -------------------------------------------------------------------------- */

//! Empty out the queue completely
PUBLIC void
eventQueueFlush( EventQueue * restrict queue )
{
    REQUIRE( queue );
    REQUIRE( queue->entries );   // Must actually have queue storage

    ASSERT(0);  // TBD This should really recycle the events that are in the
                // queue instead of just resetting the pointers.

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    queue->head  = 0;
    queue->tail  = 0;
    queue->front = NULL;
    queue->used  = 0;

    CRITICAL_SECTION_END();
}

/* ----- End ---------------------------------------------------------------- */
