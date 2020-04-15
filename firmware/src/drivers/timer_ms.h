#ifndef TIMER_MS_H
#define TIMER_MS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ---- Local Includes ------------------------------------------------------ */

#include "global.h"

/* ---- Types --------------------------------------------------------------- */

//! to declare timer variables in your code
#ifndef timer_ms_t
typedef uint32_t timer_ms_t;
#endif    // timer_t

/* ---- Public Functions ---------------------------------------------------- */

//! Start a timer for a particular timeout.
PUBLIC void
timer_ms_start( timer_ms_t *timer, timer_ms_t delay_ms );

/* -------------------------------------------------------------------------- */

//! Stop the timer from expiring.
PUBLIC void
timer_ms_stop( timer_ms_t *timer );

/* -------------------------------------------------------------------------- */

//! Return true when timer is running.
PUBLIC bool
timer_ms_is_running( timer_ms_t *timer );

/* -------------------------------------------------------------------------- */

//! Return true when timer is running and has expired.
PUBLIC bool
timer_ms_is_expired( timer_ms_t *timer );

/* -------------------------------------------------------------------------- */

//! Start a stopwatch to measure time
PUBLIC void
timer_ms_stopwatch_start( timer_ms_t *timer );

/* -------------------------------------------------------------------------- */

//! Return the current stopwatch time.
PUBLIC timer_ms_t
timer_ms_stopwatch_lap( timer_ms_t *timer );

/* -------------------------------------------------------------------------- */

//! Return the current stopwatch time.
PUBLIC timer_ms_t
timer_ms_stopwatch_stop( timer_ms_t *timer );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* TIMER_MS_H */
