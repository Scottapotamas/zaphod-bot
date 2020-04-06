/**
 * @file event_timer.h
 *
 * @brief Event Timers for State Machine Handling
 *        Support for declaring, arming and disarming timers to fire a
 *        specified event signal when the timer expires.
 *
 * @warning
 *        When the timer fires, the event queued to the task is POINTING
 *        to the Event struct. Then means that between posting the event
 *        (and the timer having been disarmed), it is possible to rearm the
 *        timer with a new event and if the previous fired event is still
 *        in the queue, it will have been changed as well!
 *
 * @author  Marco Hess <marcoh@applidyne.com.au>
 *
  * @copyright (c) 2013-2015 Applidyne Australia Pty. Ltd. - All rights reserved.
*/

#ifndef EVENT_TIMER_H
#define EVENT_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "state_task.h"

/* ----- Types -------------------------------------------------------------- */

//! Declare the size of the timer counters used.
typedef uint32_t EventTimerCounter;

typedef struct EventTimer EventTimer;
struct EventTimer
{
  const StateEvent       *timeoutEvent; //<! signal to generate upon timeout
  const StateTask        *timeoutTask;  //<! active task to deliver the event to
  EventTimer        *next;         //<! link to next timer in the list
  EventTimer        *previous;     //<! link to previous timer in the list
  EventTimerCounter counter;       //<! timer counting down to 0 to fire the event
  EventTimerCounter interval;      //<! reload value for repeating timers. 0 for single shot
};

/* ----- Public Functions --------------------------------------------------- */

//! Init the internal data for the eventTimer module
PUBLIC void
eventTimerInit( void );

//! Wrapper to allow asserting the calling application code violating
/// the usage of the timer variable so we can trap it where it happens.
#define eventTimerStartOnce( _tmr, _tsk, _e, _tim )  do {   \
          REQUIRE( ((EventTimer*)_tmr)->timeoutTask == 0 ); \
          __eventTimerStartOnce( _tmr, _tsk, _e, (const EventTimerCounter)(_tim) ); } while(0)

//! Fire a single shot timer to deliver the indicated event
PUBLIC void
__eventTimerStartOnce( EventTimer              *me,
                       const StateTask         *timeoutTask,
                       const StateEvent        *timeoutEvent,
                       const EventTimerCounter timeTicks );

//! Wrapper to allow asserting the calling application code violating
/// the usage of the timer variable so we can trap it where it happens.
#define eventTimerStartEvery( _tmr, _tsk, _e, _tim )  do {  \
          REQUIRE( ((EventTimer*)_tmr)->timeoutTask == 0 ); \
          __eventTimerStartEvery( _tmr, _tsk, _e, (const EventTimerCounter)(_tim) ); } while(0)

//! Fire a repeating timer to deliver the indicated event
PUBLIC void
__eventTimerStartEvery( EventTimer              *me,
                        const StateTask         *timeoutTask,
                        const StateEvent        *timeoutEvent,
                        const EventTimerCounter timeTicks );

//! Stop a timer firing
PUBLIC void
eventTimerStop( EventTimer *me );

//! Restart a timer with a new timeout (prevents it firing)
PUBLIC void
eventTimerRestart( EventTimer              *me,
                   EventTimerCounter const timeTicks );

//! Return TRUE when the indicated timer is active
PUBLIC bool
eventTimerIsActive( EventTimer *me );

//! Clean up the timer if it is still active
PUBLIC void
eventTimerStopIfActive( EventTimer *me );

//! eventTimerTick to be called from the system tick interrrupt handler
PUBLIC void
eventTimerTick( void );

//! Return true when there are one or more timers running
PUBLIC bool
eventTimersRunning( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* EVENT_TIMER_H */
