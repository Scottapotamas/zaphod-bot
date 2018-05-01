/**
 * @file event_timer.c
 *
 * @brief Event Timers for State Machine Handling
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "event_timer.h"
#include "state_event.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----------------------- Private Functions Declarations ------------------ */

PRIVATE void
eventTimerAdd( EventTimer              *me,
               const StateTask              *timeoutTask,
               const StateEvent             *timeoutEvent,
               const EventTimerCounter timeTicks );

PRIVATE void
eventTimerRemove( EventTimer *me );

/* ----------------------- Private Data & Variables ------------------------ */

PRIVATE EventTimer *EventTimerListHead;    // head of linked list of timers

/* ----- Public Functions --------------------------------------------------- */

//! Init the internal data for the EventTimer module
PUBLIC void
eventTimerInit( void )
{
  EventTimerListHead = NULL;
}

/* -------------------------------------------------------------------------- */

// setup a periodic timer to fire EVERY 'nTicks' clock ticks and send
// timeout message 'timeoutEvent' to active object 'timeoutTask'
PUBLIC void
__eventTimerStartEvery( EventTimer              *me,
                        const StateTask         *timeoutTask,
                        const StateEvent        *timeoutEvent,
                        const EventTimerCounter timeTicks )
{
  REQUIRE(me              != 0);
  REQUIRE(me->timeoutTask == 0); // Not in use
  REQUIRE(timeoutTask     != 0);
  REQUIRE(timeoutEvent    != 0);
  REQUIRE(timeoutEvent->dynamic.poolId   == 0);
  REQUIRE(timeoutEvent->dynamic.useCount == 0);
  me->interval = timeTicks;
  eventTimerAdd( me, timeoutTask, timeoutEvent, timeTicks );
}

/* -------------------------------------------------------------------------- */

// setup a one-shot timer to fire IN 'nTicks' clock ticks and send
// timeout message 'timeoutEvent' to active object 'timeoutTask'
PUBLIC void
__eventTimerStartOnce( EventTimer              *me,
                       const StateTask         *timeoutTask,
                       const StateEvent        *timeoutEvent,
                       const EventTimerCounter timeTicks )
{
  REQUIRE(me              != 0);
  REQUIRE(me->timeoutTask == 0); // Not in use
  REQUIRE(timeoutTask     != 0);
  REQUIRE(timeoutEvent    != 0);
  REQUIRE(timeoutEvent->dynamic.poolId   == 0);
  REQUIRE(timeoutEvent->dynamic.useCount == 0);
  me->interval = 0;
  eventTimerAdd( me, timeoutTask, timeoutEvent, timeTicks );
}

/* -------------------------------------------------------------------------- */

//! Disarm currently armed timer
PUBLIC void
eventTimerStop( EventTimer *me )
{
  // check that this timer was actually in use.
  REQUIRE(me->timeoutTask != 0);

  // eventTimerRemove has its own critical section protection
  eventTimerRemove( me );
}

/* -------------------------------------------------------------------------- */

// rearm a timer to fire in/every timeTicks
PUBLIC void
eventTimerRestart( EventTimer        *me,
                   EventTimerCounter timeTicks )
{
    REQUIRE( timeTicks > 0 );

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    me->counter = timeTicks;
    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

//! Return TRUE when the indicated timer is active
PUBLIC bool
eventTimerIsActive( EventTimer *me )
{
  return (me->timeoutTask != 0);
}

/* -------------------------------------------------------------------------- */

//! Clean up the timer if it is still active
PUBLIC void
eventTimerStopIfActive( EventTimer *me )
{
    if( eventTimerIsActive( me ) )
    {
        eventTimerStop( me );
    }
}

/* -------------------------------------------------------------------------- */

//! EventTimerTick to be called usually from ISR context
PUBLIC void
eventTimerTick( void )
{
  EventTimer *t;   //<! Current timer to evaluate

  t = EventTimerListHead;
  while( t != 0 )
  {
    if( --t->counter == 0 )  // when we reach 0 we fire the event
    {
      ASSERT( t->timeoutTask  != 0 );
      ASSERT( t->timeoutEvent != 0 );

      // Fire the event
      if( stateTaskPostFIFO( (StateTask*)t->timeoutTask,
                             t->timeoutEvent ) )
      {
        if( t->interval != 0 )       // Multishot timer?
        {
          t->counter = t->interval;  // Rearm multishot timers
        }
        else
        {
          eventTimerRemove( t );     // Remove single shot timers
        }
      }
      else
      {
        // Failed to queue the timer event. As a fallback measure, bump
        // the counter up and allow it to be tried again the next tick.
        t->counter++;
      }
    }

    // This is safe because the eventTimerRemove leaves
    // the next and previous timers alone.
    t = t->next;
  }
}

/* ----------------------- Private Functions ------------------------------- */

//! Create a new timer and link it in the list for servicing.
PRIVATE void
eventTimerAdd( EventTimer              *timer,
               const StateTask         *timeoutTask,
               const StateEvent        *timeoutEvent,
               const EventTimerCounter timeTicks )
{
    // the timer must *not* be in use
    REQUIRE( timer->timeoutTask == 0 );

    // and have valid arguments to use
    REQUIRE( timeoutTask     != 0 );
    REQUIRE( timeoutEvent    != 0 );
    REQUIRE( timeTicks       >  0 );

    // Setup the timer data
    timer->timeoutTask  = timeoutTask;
    timer->timeoutEvent = timeoutEvent;
    timer->counter      = timeTicks;

    // Link timer into the list (just at the beginning...)
    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    if( EventTimerListHead == 0 )
    {
        // New and only event in queue.

        // Make EventTimerListHead point to this event.
        EventTimerListHead = timer;

        // Clear the next and previous pointers.
        timer->next           = 0;
        timer->previous       = 0;
    }
    else
    {
        // Existing events in the queue.

        // Make this event point to the current queue head.
        timer->next           = EventTimerListHead;

        // Queue head event point to this event.
        timer->next->previous = timer;

        // Current queue head event becomes this event.
        EventTimerListHead = timer;

        // At the head of the queue, there is no previous.
        timer->previous       = 0;
    }
    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

//! Remove an existing timer from the service list
PRIVATE void eventTimerRemove( EventTimer *timer )
{
    // check that this timer was actually in use.
    REQUIRE( timer->timeoutTask != 0 );

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    // Remove timer from the tick handler chain
    if( timer->previous == 0 )
    {
        EventTimerListHead = timer->next;
    }
    else
    {
        timer->previous->next = timer->next;
    }

    if( timer->next != 0 )
    {
        timer->next->previous = timer->previous;
    }

    // Flag this timer as inactive but leave the next and
    // previous pointers alone as they are still needed in
    // eventTimerTick function to traverse the list of timers.
    // They get re-initialised when the timer is Added again.
    timer->counter      = 0;
    timer->interval     = 0;
    timer->timeoutEvent = 0;
    timer->timeoutTask  = 0;

    CRITICAL_SECTION_END();
}

/* ----- End ---------------------------------------------------------------- */
