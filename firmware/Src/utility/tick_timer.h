/**
 * @file      tick_timer.h
 *
 * @brief     The light weight and stand alone Tick Timer support services allow
 *            declaring, arming and disarming timers to fire a call-back when the
 *            timer expires.
 *
 * @author    Marco Hess <marcoh@applidyne.com.au>
 *
 * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
 */

#ifndef TICK_TIMER_H
#define TICK_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Types -------------------------------------------------------------- */

typedef void (*voidTickFireFuncPtr)(void);

//! Declare the size of the timer counters used.
typedef uint32_t TickTimerCounter;

//! \struct TickTimer
/// Structure to be declared by the application to support a tick timer. Stores the
/// relevant call back pointers, timer counters and interval reload counters as well
/// link pointers for the tick-timer package to maintain chains of active timers.
typedef struct TickTimer TickTimer;
struct TickTimer
{
  voidTickFireFuncPtr callbackOnFire; //<! callback function to deliver the event to
  TickTimer           *next;          //<! link to next timer in the list
  TickTimer           *previous;      //<! link to previous timer in the list
  TickTimerCounter    counter;        //<! timer counting down to 0 to fire the event
  TickTimerCounter    interval;       //<! reload value for repeating timers. 0 for single shot
};

/* ----- Public Functions --------------------------------------------------- */

//! Init the internal data for the tick_timer_ module
PUBLIC void
tick_timer_init( void );

//! Start a single shot timer to deliver the indicated event
PUBLIC void
tick_timer_start_once( TickTimer                 *me,
                       const voidTickFireFuncPtr timeoutCallback,
                       const TickTimerCounter    timeTicks );

//! Start a repeating timer to deliver the indicated event
PUBLIC void
tick_timer_start_every( TickTimer                 *me,
                        const voidTickFireFuncPtr timeoutCallback,
                        const TickTimerCounter    timeTicks );

//! Stop a timer firing
PUBLIC void
tick_timer_stop( TickTimer *me );

//! Disable a timer. Only to be used from within callbacks to stop the timer from
/// further firing.
PUBLIC void
tick_timer_disable( TickTimer *me );

//! Restart a timer with a new timeout (prevents it firing)
PUBLIC void
tick_timer_restart( TickTimer              *me,
                  TickTimerCounter const timeTicks );

//! Return TRUE when the indicated timer is active
PUBLIC bool
tick_timer_is_active( TickTimer *me );

//! timerTick to be called usually from ISR context
PUBLIC void
tick_timer_tick( void );

//! Return TRUE when there are one or more timers running
PUBLIC bool
tick_timer_running( void );

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* TICK_TIMER_H */

