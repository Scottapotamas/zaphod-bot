#ifndef STOPWATCH_H
#define STOPWATCH_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ---- Local Includes ------------------------------------------------------ */

#include "global.h"

/* ---- Types --------------------------------------------------------------- */

//! to declare timer variables in your code
#ifndef stopwatch_t
typedef uint32_t stopwatch_t;
#endif    // timer_t

/* ---- Public Functions ---------------------------------------------------- */

//! Start a timer for a particular timeout.
PUBLIC void
stopwatch_deadline_start( stopwatch_t *timer, stopwatch_t delay_ms );

/* -------------------------------------------------------------------------- */

//! Stop the timer from expiring.
PUBLIC void
stopwatch_deadline_stop( stopwatch_t *timer );

/* -------------------------------------------------------------------------- */

//! Return true when timer is running.
PUBLIC bool
stopwatch_deadline_pending( stopwatch_t *timer );

/* -------------------------------------------------------------------------- */

//! Return true when timer is running and has expired.
PUBLIC bool
stopwatch_deadline_elapsed( stopwatch_t *timer );

/* -------------------------------------------------------------------------- */

//! Start a stopwatch to measure time
PUBLIC void
stopwatch_start( stopwatch_t *timer );

/* -------------------------------------------------------------------------- */

//! Return the current stopwatch time.
PUBLIC stopwatch_t
stopwatch_lap( stopwatch_t *timer );

/* -------------------------------------------------------------------------- */

//! Stop the timer.
PUBLIC void
stopwatch_stop( stopwatch_t *timer );


/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* STOPWATCH_H */
