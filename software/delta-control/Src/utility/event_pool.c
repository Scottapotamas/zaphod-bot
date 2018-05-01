/**
 * @file event_pool.c
 *
 * @brief Event Pool Managment.
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "event_pool.h"
#include "qassert.h"
#include "state_task.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----------------------- Private Functions Declarations ------------------- */

PRIVATE EventPool *
eventPoolCreate( EventPool *me,
                 StateEvent     *poolStorage,
                 uint16_t  numberOfEvents,
                 uint16_t  eventSize );

PRIVATE StateEvent *
eventPoolGet( EventPool *me );

PRIVATE void
eventPoolPut( EventPool *me, StateEvent *e );

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~        Private Data & Variables                                       ~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

//! Specify EVENT_POOL_MAX if it was not defined before
//#ifndef EVENT_POOL_MAX
//  #define EVENT_POOL_MAX  3  //<! total number of pools allowed
//  #warning "Allocating EVENT_POOL_MAX with default 3 pools"
//#endif
//#if EVENT_POOL_MAX > 3
//  #error "Cannot allocate more than 3 event pools. See size of Event.dynamic.poolId"
//#endif

//! Allocate EVENT_POOL_MAX event pools (see 'event_pool.h')
//PRIVATE EventPool locPool[EVENT_POOL_MAX];

//! The pool pointers keep track of pools actually used.
/// The first and last poolPtr are not used (must be 0),
/// which is guaranteed by static initialization in C/C++.
//PRIVATE EventPool *locPoolPtr[1 + EVENT_POOL_MAX + 1];


//PRIVATE int8u poolId = 0;

/* -------------------------------------------------------------------------- */

// Remember the maximum number of pools we have.
PRIVATE uint8_t   eventPoolMax;

// Remember the pointer to the event pool array
PRIVATE EventPool *eventPool;

/* ----- Public Functions --------------------------------------------------- */

//! Initialise a pool into a pool table and set it up with a pool id.
/// Please initialise event pools in ascending order of eventSize
PUBLIC void
eventPoolInit( EventPool eventPools[],
               uint8_t   numberOfPools )
{
    uint8_t i;

    REQUIRE( eventPools );
    REQUIRE( numberOfPools > 0 );

    // Init the EventPool structures for the whole array
    for( i = 0; i < numberOfPools; i++ )
    {
        memset( &eventPools[i], 0, sizeof( EventPool) );
    }

    // Remember the location and count
    eventPoolMax = numberOfPools;
    eventPool    = &eventPools[0];
}

/* -------------------------------------------------------------------------- */

//! Add storage for a particular event size to the pool table initialised
/// with eventPoolInit.
/// Please add event pools storage in ascending order of eventSize
/// Returns the pool Id when successfully added or 0 when it was unable
/// to add the pool storage.
PUBLIC uint8_t
eventPoolAddStorage( StateEvent     *poolStorage,
                     uint16_t  numberOfEvents,
                     uint16_t  eventEntrySize )
{
    uint8_t i;

    REQUIRE( poolStorage );
    REQUIRE( numberOfEvents > 0 );
    REQUIRE( eventEntrySize > 0 );

    for( i = 0; i < eventPoolMax; i++ )
    {
        // Look for unused pool entries
        if( eventPool[i].eventSize == 0 )
        {
            eventPoolCreate( &eventPool[i],
                             poolStorage,
                             numberOfEvents,
                             eventEntrySize );
            return i+1;
        }
    }
    return 0;
}

/* -------------------------------------------------------------------------- */

//! Allocate a new event memory block from a set of pools
/// Could be returning NULL if no event was available from the pools
PUBLIC StateEvent *
eventPoolNewEvent( uint16_t eventSize, Signal signal )
{
    uint8_t i;

    REQUIRE( eventSize > 0 );

    for( i = 0; i < eventPoolMax; i++ )
    {
        EventPool *p = &eventPool[i];

        // is this pool of events of a suitable size
        // for this event?
        if( (p->eventSize > 0) && (p->eventSize >= eventSize) )
        {
            // Try allocating one from this pool. If exhausted we
            // allocate from the next pool with bigger events.
            StateEvent *e = eventPoolGet( p );
            if( e )
            {
                // We got an event allocated, so record the details with
                // the event.
                e->signal           = signal; // set signal for this event
                e->dynamic.poolId   = i + 1;  // to know where to recycle this event
                e->dynamic.useCount = 0;      // this event is new, not used yet
                return e;
            }
        }
    }

    // Tried to get a too big event for these pools. Check the configuration
    // of the large pool event size.
    ASSERT( eventSize <= eventPool[eventPoolMax-1].eventSize );

    // Ran out of memory - either the pools are too small or
    // something is not freeing the events.
    ASSERT( false );

    // When NASSERT is defined, the ASSERTs are disabled and we return NULL.
    return 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
eventPoolDeleteEvent( StateEvent *e )
{
    if( e )
    {
        // is it a pool event?
        if( e->dynamic.poolId != 0 )
        {
            ASSERT( (e->dynamic.poolId-1 < eventPoolMax)
                    &&
                    (eventPool[e->dynamic.poolId-1].eventSize > 0) );
            eventPoolPut( &eventPool[e->dynamic.poolId-1], e );
        }
        else // not a pool event
        {
            // recycle by clearing the number of uses
            e->dynamic.useCount = 0;
        }
    }
}

/* -------------------------------------------------------------------------- */

/** After a state machine has executed an event, this function decrements
 *  the usage counter on the event and deletes it when usage becomes 0.
 */
PUBLIC void
eventPoolGarbageCollect( StateEvent *e )
{
    if( e->dynamic.poolId > 0 )
    {
        // It must have been in use
        REQUIRE(e->dynamic.useCount > 0);

        // One less reference for this event
        e->dynamic.useCount--;

        // If no more references, deallocate the event
        if(e->dynamic.useCount == 0)
        {
            eventPoolDeleteEvent( e );
        }
    }
}

/* ----------------------- Private Functions ------------------------------- */

//! Initialise a pool structure and set it up with a linked list
/// of free events.
PRIVATE EventPool *
eventPoolCreate( EventPool  *pool,
                 StateEvent *poolStorage,
                 uint16_t   numberOfEvents,
                 uint16_t   eventEntrySize )
{
    register char *block;

    REQUIRE( (numberOfEvents > 0) && (eventEntrySize >= sizeof(StateEvent)) );

    pool->free__           = poolStorage;    // set head of linked-list of free events
    pool->eventSize        = eventEntrySize; // store maximum size of each event
    pool->totalEvents      = numberOfEvents; // store total number of events
    pool->freeEvents       = numberOfEvents; // store number of free events
    pool->minimumEvents    = numberOfEvents; // the minimum number of free events
    pool->minimumEventSize = 0;              // the actual used maximum size for events in this pool

    block = (char *)poolStorage;
    while (--numberOfEvents != 0)          // chain all blocks in the free-list...
    {
        *(void **)block = (void *)(block + eventEntrySize);
        block += eventEntrySize;
    }
    *(void **)block = 0;                   // last link points to 0

    return pool;
}

/* -------------------------------------------------------------------------- */

PRIVATE StateEvent *
eventPoolGet( EventPool *pool )
{
    register StateEvent *e;

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    if( pool->freeEvents > 0 )             // free block available?
    {
        e = (StateEvent*)pool->free__;          // get free block
        pool->free__ = *(void **)e;        // adjust ptr to new free list

        --pool->freeEvents;                // one less event

        // remember the minimum so far for
        // later storage optimisation
        if (pool->freeEvents < pool->minimumEvents)
        {
            pool->minimumEvents = pool->freeEvents;
        }
    }
    else                                 // allocation failed
    {
        e = 0;
    }

    CRITICAL_SECTION_END();

    return e;         // return event or NULL pointer to the caller */
}

/* -------------------------------------------------------------------------- */

PRIVATE void
eventPoolPut( EventPool *pool, StateEvent *e )
{
    // #of free blks must be < total
    REQUIRE( pool->freeEvents < pool->totalEvents );

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    *(void **)e = pool->free__;   // insert released block into free list
    pool->free__ = e;             // set as new head of the free list
    ++pool->freeEvents;           // one more block in this pool

    CRITICAL_SECTION_END();
}

/* ----- End ---------------------------------------------------------------- */
