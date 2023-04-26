#ifndef HAL_CORE_H
#define HAL_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void hal_core_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void hal_core_clock_configure( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_CORE_H */
