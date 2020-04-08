/**
 * @file event_subscribe.c
 *
 * @brief Event queue subscription and publishing support
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2016 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "qassert.h"
#include "event_subscribe.h"
#include "bitset.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----- Variables ---------------------------------------------------------- */

PRIVATE EventSubscribers *eventSubscribersList; /** pointer to signals subscriptions table */
PRIVATE uint8_t          locMaxSignal;          /** max signals in list */
PRIVATE StateTask        **eventTaskTable;      /** Table of tasks pointers */

/* ----- Public Functions --------------------------------------------------- */

//! Init the event subscribers table
PUBLIC void
eventSubscribeInit( StateTask      **tasksTable,
                    EventSubscribers subscribersList[],
                    const uint8_t    maxSignals )
{
    REQUIRE( subscribersList );            // Is this pointing to something
    REQUIRE( maxSignals > 0 );

    eventTaskTable       = tasksTable;
    eventSubscribersList = subscribersList;
    locMaxSignal         = maxSignals;

    // Clean out the subscribers table
    memset( eventSubscribersList, 0, sizeof(EventSubscribers) * maxSignals );
}

/* -------------------------------------------------------------------------- */

//! Register a subscription to an event
PUBLIC void
eventSubscribe( StateTask    *t,
                const Signal signal )
{
    REQUIRE( t );                           // Is this pointing to something
    REQUIRE( STATE_USER_SIGNAL <= signal ); // Signal is a user signal
    REQUIRE( signal < locMaxSignal );       // Signal is within bounds
    REQUIRE( t->tasker );                   // Is this an active task?

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    bitsetSet( &eventSubscribersList[signal], t->id );
    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

//! Unregister a subscription to an event
PUBLIC void
eventUnsubscribe( StateTask    *t,
                  const Signal signal )
{
    REQUIRE( t );                           // Is this pointing to something
    REQUIRE( STATE_USER_SIGNAL <= signal ); // Signal is a user signal
    REQUIRE( signal < locMaxSignal );       // Signal is within bounds
    REQUIRE( t->tasker );                   // Is this an active task?

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    bitsetClear( &eventSubscribersList[signal], t->id );
    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

//! Publish an event to the subscribers list. When there are no subscribers
/// to this event, or none of the subscriber queues can take the event, the
/// event is recycled.
/// Without asserts, a NULL event is silently ignored.
/// Returns true when event is delivered to all subscribers (incl. when
/// there are no subscribers at all). False when there was a problem with
/// a full queue on one of the subscribed tasks.
PUBLIC bool
eventPublish( const StateEvent *e )
{
    register bool fully_delivered = true;
    EventSubscribers eventSubscribers;

    REQUIRE( e );                           // Is this pointing to something
    REQUIRE( e->signal < locMaxSignal );    // Is this a valid signal?
    REQUIRE( e->dynamic.useCount == 0 );    // Don't publish an event that
                                            // is already in use!

    if( e ) // Silently ignore NULL events if asserts are not used.
    {
        // Lookup the subscribers list for this event
        eventSubscribers = eventSubscribersList[e->signal];

        if( eventSubscribers != 0 ) // any subscribers?
        {
            bool success = false;

            while( eventSubscribers > 0 )
            {
                register uint8_t p;
                p = bitsetHighest( &eventSubscribers );
                bitsetClear( &eventSubscribers, p );
                ASSERT( eventTaskTable[p] );  // check if task is active
                                              // check queue can take event
                if( stateTaskPostFIFO( eventTaskTable[ p ], e ) )
                {
                    // Keep track that event is in on a task queue now.
                    // So we don't have to recycle it below.
                    success = true;
                }
                else
                {
                    // Failed to deliver event to subscribed queue
                    // (queue is full)
                    fully_delivered = false;
//                    ASSERT_PRINTF( false, "e=%d, t=%d", e->signal, p );
                }
            }

            // None of the publishing queues were able to take the event
            // so ensure that we recycle it
            if( !success )
            {
                EVENT_DELETE( e );
            }
        }
        else // no subscribers, recycle immediately
        {
            EVENT_DELETE( e );
        }
    }
    return fully_delivered;
}

/* ----- End ---------------------------------------------------------------- */
