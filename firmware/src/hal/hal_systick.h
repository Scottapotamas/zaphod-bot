#ifndef HAL_SYSTICK_H
#define HAL_SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

/* Up to 5 hooks can be used. Ensure this is big enough for the number used */
#define HAL_SYSTICK_MAX_HOOKS 5

/* -------------------------------------------------------------------------- */

typedef void ( *voidTickHookFuncPtr )( void );

/* -------------------------------------------------------------------------- */

/** Ensure that the timer needed for the delay measurement is started */

PUBLIC void
hal_systick_init( void );

/* -------------------------------------------------------------------------- */

/** Provides a tick value in millisecond.*/

PUBLIC uint32_t
hal_systick_get_ms( void );

/* -------------------------------------------------------------------------- */

// Add a callback function to the 1ms tick timer. Returns true when
// hook was successfully added. The count indicates the tick rate at which the
// hooked function runs.

PUBLIC bool
hal_systick_hook( uint32_t count, voidTickHookFuncPtr hookfunc );

/* -------------------------------------------------------------------------- */

// Remove a callback function from the 1ms tick timer

PUBLIC void
hal_systick_unhook( voidTickHookFuncPtr hookfunc );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_SYSTICK_H */
