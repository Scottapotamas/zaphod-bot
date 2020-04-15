/* ---- Local Includes ------------------------------------------------------ */

#include "timer_ms.h"
#include "hal_systick.h"

/* ---- Public Functions ---------------------------------------------------- */

//! Start a timer for a particular timeout.
PUBLIC void
timer_ms_start( timer_ms_t *timer, timer_ms_t delay_ms )
{
    *timer = hal_systick_get_ms() + delay_ms;
}

/* -------------------------------------------------------------------------- */

//! Stop the timer from expiring.
PUBLIC void
timer_ms_stop( timer_ms_t *timer )
{
    *timer = 0;
}

/* -------------------------------------------------------------------------- */

//! Return true when timer is running.
PUBLIC bool
timer_ms_is_running( timer_ms_t *timer )
{
    return ( *timer ) != 0;
}

/* -------------------------------------------------------------------------- */

//! Return true when timer is running and has expired.
//! Return false when timer is stopped or when it has not expired.
PUBLIC bool
timer_ms_is_expired( timer_ms_t *timer )
{
    return ( *timer ) && ( hal_systick_get_ms() >= *timer );
}

/* -------------------------------------------------------------------------- */

//! Start a stopwatch to measure time
PUBLIC void
timer_ms_stopwatch_start( timer_ms_t *timer )
{
    *timer = hal_systick_get_ms();
}

/* -------------------------------------------------------------------------- */

//! Return the current stopwatch time.
PUBLIC timer_ms_t
timer_ms_stopwatch_lap( timer_ms_t *timer )
{
    return hal_systick_get_ms() - *timer;
}

/* -------------------------------------------------------------------------- */

//! Return the current stopwatch time.
PUBLIC timer_ms_t
timer_ms_stopwatch_stop( timer_ms_t *timer )
{
    return hal_systick_get_ms() - *timer;
}

/* ----- End ---------------------------------------------------------------- */
