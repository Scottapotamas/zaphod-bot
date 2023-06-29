#ifndef HAL_DEBUG_H
#define HAL_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void hal_debug_printf( const char *fmt, ... );

/* -------------------------------------------------------------------------- */

PUBLIC void hal_debug_print( const char *fmt );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t hal_debug_get_dwt( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif    // HAL_DEBUG_H