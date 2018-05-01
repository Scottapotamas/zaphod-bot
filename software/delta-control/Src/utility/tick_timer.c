/**
 * @file      tick_timer.c
 *
 * @brief     The light weight and stand alone Tick Timer support services allow
 *            declaring, arming and disarming timers to fire a call-back when the
 *            timer expires.
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

/* ----- System Includes ---------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "qassert.h"
#include "tick_timer.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* ----------------------- Private Functions ------------------------------- */

PRIVATE void tick_timer_add( TickTimer                 *me,
                             const voidTickFireFuncPtr timeoutCallback,
                             const TickTimerCounter    timeTicks );

PRIVATE void tick_timer_remove( TickTimer *me );

/* ----------------------- Private Data ------------------------------------ */

//! Head of linked list of timers
PRIVATE TickTimer *TickTimerListHead;

/* ----- Public Functions --------------------------------------------------- */

//! Init the internal data for the tick_timer_ module
PUBLIC void
tick_timer_init( void )
{
    TickTimerListHead = NULL;
}

/* -------------------------------------------------------------------------- */

// setup a periodic timer to fire EVERY 'nTicks' clock ticks and send
// timeout message 'toutSig' to active object 'act'
PUBLIC void
tick_timer_start_every( TickTimer                 *me,
                        const voidTickFireFuncPtr timeoutCallback,
                        const TickTimerCounter    timeTicks )
{
    REQUIRE( me                 != 0 );
    REQUIRE( me->callbackOnFire == 0 ); // Not in use
    REQUIRE( timeoutCallback    != 0 );

    me->interval = timeTicks;
    tick_timer_add( me, timeoutCallback, timeTicks );
}

/* -------------------------------------------------------------------------- */

// setup a one-shot timer to fire in 'timeTicks' clock ticks and send
// timeout message 'timeoutEvent' to 'timeoutTask'
PUBLIC void
tick_timer_start_once( TickTimer                 *me,
                       const voidTickFireFuncPtr timeoutCallback,
                       const TickTimerCounter    timeTicks )
{
    REQUIRE( me                 != 0 );
    REQUIRE( me->callbackOnFire == 0 ); // TickTimer task is already pointing to something...
    REQUIRE( timeoutCallback    != 0 );

    me->interval = 0;
    tick_timer_add( me, timeoutCallback, timeTicks );
}

/* -------------------------------------------------------------------------- */

//! Disarm currently armed timer
PUBLIC void
tick_timer_stop( TickTimer *me )
{
    // check that this function was not actually called from within a timer
    // callback function.
    REQUIRE( me->counter != 0 );

    // check that this timer was actually in use.
    REQUIRE( me->callbackOnFire != 0 );

    // tick_timer_Remove has its own critical section protection
    tick_timer_remove( me );
}

/* -------------------------------------------------------------------------- */

//! When you try to disarm a tick timer from within the callback functions
/// it becomes a problem, because the timer is removed and then the tick handler
/// thinks it is a one shot timer and tries to remove it again. This function
/// merrily disables the interval counter and then on the return of the callback
/// the tick handler safely removes the timer.
PUBLIC void
tick_timer_disable( TickTimer *me )
{
    // check that this function was actually called from within a callback
    // function where the counter is 0.
    REQUIRE( me->counter == 0 );

    // Just clear the interval and let the tick handler clean it up.
    me->interval = 0;
}

/* -------------------------------------------------------------------------- */

// rearm a timer to fire in/every nTicks (if 'nTicks'==0 the old
// value is restored)
PUBLIC void
tick_timer_restart( TickTimer        *me,
                    TickTimerCounter timeTicks )
{
    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    me->counter = timeTicks;
    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

//! Return TRUE when the indicated timer is active
PUBLIC bool
tick_timer_is_active( TickTimer *me )
{
    return ( me->counter != 0 );
}

/* -------------------------------------------------------------------------- */

//! tick_timer_Tick to be called usually from ISR context
PUBLIC void
tick_timer_tick( void )
{
    register TickTimer *t;   //<! Current timer to evaluate

    t = TickTimerListHead;
    while( t != 0 )
    {
        if( --t->counter == 0 )  // when we reach 0 we fire the event
        {
            // Fire the event
            ASSERT( t->callbackOnFire != 0 );
            (t->callbackOnFire)();

            if( t->interval != 0 )          // Multishot timer?
            {
                t->counter = t->interval;   // Rearm multishot timers
            }
            else
            {
                tick_timer_remove( t );     // Remove single shot timers
            }
        }

        // This is safe because the tick_timer_remove leaves
        // the next and previous timers alone.
        t = t->next;
    }
}

/* -------------------------------------------------------------------------- */

//! Create a new timer and link it in the list for servicing.
PRIVATE void
tick_timer_add( TickTimer                 *me,
                const voidTickFireFuncPtr timeoutCallback,
                const TickTimerCounter    timeTicks )
{
    // the timer must *not* be in use
    REQUIRE( me->callbackOnFire == 0 );

    // and have valid arguments to use
    REQUIRE( timeoutCallback != 0 );
    REQUIRE( timeTicks       >  0 );

    // setup the timer data
    me->callbackOnFire = timeoutCallback;
    me->counter        = timeTicks; // Interval is pre-initialised before
                                    // the call to this Add function.

    // link timer into the list (just at the beginning...)
    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();
    if( TickTimerListHead == 0 )
    {
        TickTimerListHead = me;
        me->next           = 0;
        me->previous       = 0;
    }
    else
    {
        me->next           = TickTimerListHead;
        me->next->previous = me;
        TickTimerListHead  = me;
        me->previous       = 0;
    }
    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

//! Remove an existing timer from the service list
PRIVATE void
tick_timer_remove( TickTimer *me )
{
    // check that this timer was actually in use.
    REQUIRE( me->callbackOnFire != 0 );

    // Remove timer from the tick handler chain
    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    if( me->previous == 0 )
    {
        TickTimerListHead  = me->next;
    }
    else
    {
        me->previous->next = me->next;
    }

    if( me->next != 0 )
    {
        me->next->previous = me->previous;
    }

    // Flag this timer as inactive but leave the next and
    // previous pointers alone as they are still needed in
    // tick_timer_Tick function to traverse the list of timers.
    // They get re initialised when the timer is Added again.
    me->counter        = 0;
    me->interval       = 0;
    me->callbackOnFire = 0;

    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

//! Return TRUE when there are one or more timers running
PUBLIC bool
tick_timer_running(void)
{
    // When there are no timers on the active list
    // we can stop the clocks without problems.
    return TickTimerListHead != 0;
}

/* ----------------------- End --------------------------------------------- */
