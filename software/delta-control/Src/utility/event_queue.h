/**
 * @file    event_queue.h
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

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "state_event.h" // for Event type

/* ----- Types -------------------------------------------------------------- */

typedef struct EventQueue EventQueue;
struct EventQueue
{
    uint8_t     size;         ///< pointer to the end of the ring buffer
    uint8_t     head;         ///< pointer to where next event will be inserted
    uint8_t     tail;         ///< pointer to where next event will be extracted

    uint8_t     nTotal;       ///< total # of entries in the buffer
    uint8_t     used;         ///< # of elements used in the buffer
    uint8_t     max;          ///< maximum # of events ever in the buffer
    StateEvent  * restrict front;       ///< pointer to event at the front of the queue
    StateEvent  * restrict *entries;    ///< pointer to event pointer array
};

/* ----- Public Functions --------------------------------------------------- */

//! Initialise the queue structure
PUBLIC EventQueue *
eventQueueInit( EventQueue * restrict queue,
                StateEvent * restrict queueStorage[],
                uint8_t    num_entries );

//! Return the number events in the queue. 0 when none
PUBLIC uint8_t
eventQueueUsed( EventQueue * restrict queue );

//! Retrieve an event from the queue
PUBLIC StateEvent *
eventQueueGet( EventQueue * restrict queue );

//! Return the event on the front of the queue but leave it in the queue
PUBLIC StateEvent *
eventQueuePeek( EventQueue * restrict queue );

//! Add an event to the tail of the queue so it comes out after existing events
PUBLIC bool
eventQueuePutFIFO( EventQueue * restrict queue, StateEvent * restrict e );

//! Add an event to the head of the queue so it comes out first
PUBLIC bool
eventQueuePutLIFO( EventQueue * restrict queue, StateEvent *ve );

//! Empty out the queue completely
PUBLIC void
eventQueueFlush( EventQueue * restrict queue );

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* EVENT_QUEUE_H */
