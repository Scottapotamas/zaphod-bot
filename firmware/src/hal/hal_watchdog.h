#ifndef HAL_WATCHDOG_H
#define HAL_WATCHDOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

/** Init the watchdog. The IWDG is clocked from the internal LSI RC
 *  oscilator and can generate timeouts between 0.1ms and 25.5s.
 *
 *  Here we set it to a prescale of 128 for approx. 4ms ticks
 *
 *  This can generate a timeout between 4 and around 16 seconds.
 *
 */

PUBLIC void
hal_watchdog_init( uint32_t timeout_ms );

/* -------------------------------------------------------------------------- */

/** Kick the dog. */

PUBLIC void
hal_watchdog_refresh( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_WATCHDOG_H */
