/**
 * @file event_subscribe.h
 *
 * @brief The Event Publisher services support the tasks subscribing to
 *        specified events and publishing events. In this way tasks don't
 *        have to know who is a consumer of a particular event. They can
 *        just publish it and if there is a task interested in the event,
 *        the event will automatically get delivered to one or more of the
 *        subscribers.
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
  * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
*/

#ifndef EVENT_SUBSCRIBE_H
#define EVENT_SUBSCRIBE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "state_task.h"
#include "bitset.h"

/* ----- Types -------------------------------------------------------------- */

//! Bitmap of subscribed tasks per signal (e.g. up to 8 tasks supported)
typedef BitSet_t EventSubscribers;

/* ----- Public Functions --------------------------------------------------- */

//! Init the event subscribers table
PUBLIC void
eventSubscribeInit( StateTask      **tasksTable,
                    EventSubscribers subscribersList[],
                    const uint8_t    maxSignals );

//! Register a subscription to an event
PUBLIC void
eventSubscribe( StateTask         *t,
                const Signal signal );

//! Unregister a subscription to an event
PUBLIC void
eventUnsubscribe( StateTask         *t,
                  const Signal signal );

//! Publish an event to the subscribers list. When there are no subscribers
/// to this event, or none of the subscriber queues can take the event, the
/// event is recycled.
/// Without asserts, a NULL event is silently ignored.
/// Returns true when event is delivered to all subscribers (incl. when
/// there are no subscribers at all). False when there was a problem with
/// a full queue on one of the subscribed tasks.
PUBLIC bool
eventPublish( const StateEvent *e );

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* EVENT_SUBSCRIBE_H */
