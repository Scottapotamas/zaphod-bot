/* -------------------------------------------------------------------------- */

#include "hal_debug.h"

#include "stm32f4xx_ll_cortex.h"

// String print requirements
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
char itm_buffer[100] = { 0 };

PUBLIC void hal_debug_printf( const char *fmt, ... )
{
    va_list args;
    char *ptr = &itm_buffer[0];

    va_start(args, fmt);
    vsnprintf( ptr, sizeof( itm_buffer ), fmt, args );
    va_end(args);

    while (*ptr != '\0')
    {
        ITM_SendChar(*ptr++);
    }
//    memset( &itm_buffer, 0, sizeof(itm_buffer) );
}

PUBLIC void hal_debug_print( const char *fmt )
{
    while (*fmt != '\0')
    {
        ITM_SendChar(*fmt++);
    }
}

PUBLIC uint32_t hal_debug_get_dwt( void )
{
    return DWT->CYCCNT;
}

/* -------------------------------------------------------------------------- */