/* ---- Local Includes ------------------------------------------------------ */

#include "stopwatch.h"
#include "FreeRTOS.h"
#include "timers.h"

/* ---- Public Functions ---------------------------------------------------- */

//! Start a timer for a particular timeout.
PUBLIC void
stopwatch_deadline_start( stopwatch_t *timer, stopwatch_t delay_ms )
{
    *timer = xTaskGetTickCount() + pdMS_TO_TICKS(delay_ms);
}

/* -------------------------------------------------------------------------- */

//! Stop the timer from expiring.
PUBLIC void
stopwatch_deadline_stop( stopwatch_t *timer )
{
    *timer = 0;
}

/* -------------------------------------------------------------------------- */

//! Return true when timer is running.
PUBLIC bool
stopwatch_deadline_pending( stopwatch_t *timer )
{
    return ( *timer ) != 0;
}

/* -------------------------------------------------------------------------- */

//! Return true when timer is running and has expired.
//! Return false when timer is stopped or when it has not expired.
PUBLIC bool
stopwatch_deadline_elapsed( stopwatch_t *timer )
{
    return ( *timer ) && ( xTaskGetTickCount() >= *timer );
}

/* -------------------------------------------------------------------------- */

//! Start a stopwatch to measure time
PUBLIC void
stopwatch_start( stopwatch_t *timer )
{
    *timer = xTaskGetTickCount();
}

/* -------------------------------------------------------------------------- */

//! Return the current stopwatch time in ms.
PUBLIC stopwatch_t
stopwatch_lap( stopwatch_t *timer )
{
    return ((xTaskGetTickCount() - *timer) * configTICK_RATE_HZ ) / 1000U;
}

/* -------------------------------------------------------------------------- */

//! Stop the timer.
PUBLIC void
stopwatch_stop( stopwatch_t *timer )
{
    *timer = 0;
}

/* ----- End ---------------------------------------------------------------- */
