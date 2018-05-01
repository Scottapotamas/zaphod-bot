/**
 * @file event_pool.h
 *
 * @brief The Event Pool services support the initialisation, getting and
 *        putting event storage from a memory pool. Typically multiple event
 *        pools (up to 3) are in use with different block sizes and event
 *        requests are allocated from the first pool that has an event
 *        available of the required size.
 *
 * Usage:
 *   1) Define the different event types that you want to allocate pools for
 *   2) Define arrays of these event types of the required size
 *   3) Call eventPoolInit as in:
 *      eventPoolInit(myEventArray, DIM(myEventArray), sizeof(myEvent));
 *   4) In the application call 'EVENT_NEW(myEvent, MY_EVENT_SIGNAL)' to
 *      allocate an entry in the memory pool for the new event. Or use
 *      EVENT_NEW_EXTRA(myEvent, extra_data_size, MY_EVENT_SIGNAL) to allocate
 *      an event type that uses a variable size buffer size. e.g. a message
 *      string. Such event structures can be declared with a last member of
 *      type char[1] to signify a variable string/byte buffer
 *   5) Call 'eventPoolDeleteEvent(myEvent)' when the event is no longer
 *      needed (this happens automatically when there are no more subscribers
 *      to the event).
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef EVENT_POOL_H
#define EVENT_POOL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "state_event.h"

/* ----- Types -------------------------------------------------------------- */

typedef struct EventPool EventPool;
struct EventPool
{
  void     *free__;          //!< linked list of free blocks
  uint16_t eventSize;        //!< maximum event size (in bytes)
  uint16_t totalEvents;      //!< total number of events in pool
  uint16_t freeEvents;       //!< number of free blocks remaining
  uint16_t minimumEvents;    //!< minimum number of free blocks
  uint16_t minimumEventSize; //!< minimum used size for event (in bytes)
};

/* ----- Public Functions --------------------------------------------------- */

/** Initialise a event pool table.
 *  Please initialize event pools in ascending order of eventSize
 */

PUBLIC void
eventPoolInit( EventPool eventPool[],
               uint8_t   numberOfPools );

/* -------------------------------------------------------------------------- */

/** Add storage for a particular event size to the pool table initialised
 *  with eventPoolInit.
 *  Please add event pools storage in ascending order of eventSize
 *  Returns the pool Id when successfully added or 0 when it was unable
 *  to add the pool storage.
 */

PUBLIC uint8_t
eventPoolAddStorage( StateEvent   *poolStorage,
                     uint16_t numberOfEvents,
                     uint16_t eventSize );

/* -------------------------------------------------------------------------- */

/** Returns a pointer to an allocated event memory block of the required
 *  size. The event is allocated on a smallest available fit from the set
 *  of available event pools. Primarily called through the EVENT_NEW macro.
 */
PUBLIC StateEvent *
eventPoolNewEvent( uint16_t eventSize, Signal signal );

/** Macro to allocate a new event memory block and automatically
 *  set the size of the event type required and cast the result
 *  into the right type.
 */
#define EVENT_NEW( evtType_, sig_ ) \
       (evtType_ *)eventPoolNewEvent( sizeof(evtType_), (sig_) )

/** Macro to allocate a new event memory block and dynamically
 *  specify the size of the event, and cast the result
 *  into the right type. Used for dynamically sized event types.
 */
#define EVENT_NEW_EXTRA( evtType_, extra_, sig_ ) \
       (evtType_ *)eventPoolNewEvent( sizeof(evtType_) + (extra_) , (sig_) )

/* -------------------------------------------------------------------------- */

/** Macro to delete a previously allocate but unused event.
 *  Really more for consistency with the EVENT_NEW macros.
 */
#define EVENT_DELETE( state_event_ ) \
       eventPoolDeleteEvent( (StateEvent*)state_event_ );

/** Discard an event and return it to the memory pool
 */
PUBLIC void
eventPoolDeleteEvent( StateEvent *e );

/* -------------------------------------------------------------------------- */

/** After a state machine has executed an event, this function decrements
 *  the usage counter on the event and deletes it when usage becomes 0.
 */
PUBLIC void
eventPoolGarbageCollect( StateEvent *e );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* EVENT_POOL_H */
