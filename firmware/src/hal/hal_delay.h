#ifndef HAL_DELAY_H
#define HAL_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

/** Delay time in milli seconds. Uses a background timer for timing. */

PUBLIC void
hal_delay_ms( uint32_t delay_ms );

/* -------------------------------------------------------------------------- */

/** Delay time in micro seconds. Uses a background timer for timing. */

PUBLIC void
hal_delay_us( uint32_t delay_us );

/* -------------------------------------------------------------------------- */

/** Delay time in nano seconds. Uses a background timer for timing. */

PUBLIC void
hal_delay_ns( uint32_t delay_ns );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_DELAY_H */
