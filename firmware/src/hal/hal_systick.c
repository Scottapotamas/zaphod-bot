/* -------------------------------------------------------------------------- */

#include <string.h>

/* -------------------------------------------------------------------------- */

#include "hal_systick.h"
#include "qassert.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_utils.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

typedef struct
{
    uint32_t            count;
    uint32_t            ticks;
    voidTickHookFuncPtr func;
} TickHook_t;

/* Ensure the hook pointers are all init before main starts */
PRIVATE TickHook_t tick_hooks[HAL_SYSTICK_MAX_HOOKS] = { { 0 } };

uint32_t tick_timer = 0;

/* -------------------------------------------------------------------------- */

/** Enable and init system tick. Configure for the current system clock */

PUBLIC void
hal_systick_init( void )
{
    /* Use systick as time base source and configure 1ms tick
     * (default clock after Reset is HSI) */

    // Called earlier by LL_Init1msTick() in the main system clock setup stage
    //    LL_InitTick( rcc_clks.HCLK_Frequency, 1000U );
    tick_timer = 0;

    LL_SYSTICK_EnableIT();
}

/* -------------------------------------------------------------------------- */

/** Provides a tick value in millisecond.*/

PUBLIC uint32_t
hal_systick_get_ms( void )
{
    return tick_timer;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
hal_systick_hook( uint32_t count, voidTickHookFuncPtr hookfunc )
{
    // Make sure it is not already in the list
    hal_systick_unhook( hookfunc );

    bool empty_available = false;

    // Add the output function in the list
    for( uint8_t handler = 0; handler < HAL_SYSTICK_MAX_HOOKS; handler++ )
    {
        if( tick_hooks[handler].func == NULL )
        {
            empty_available = true;
            CRITICAL_SECTION_VAR();
            CRITICAL_SECTION_START();
            tick_hooks[handler].count = count;
            tick_hooks[handler].ticks = 0;
            tick_hooks[handler].func  = hookfunc;
            CRITICAL_SECTION_END();
            break;
        }
    }
    ENSURE( empty_available );

    return empty_available;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_systick_unhook( voidTickHookFuncPtr hookfunc )
{
    for( uint8_t handler = 0; handler < HAL_SYSTICK_MAX_HOOKS; handler++ )
    {
        if( tick_hooks[handler].func == hookfunc )
        {
            CRITICAL_SECTION_VAR();
            CRITICAL_SECTION_START();
            tick_hooks[handler].func = NULL;
            CRITICAL_SECTION_END();
            break;
        }
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_systick_callback( void )
{
    for( uint8_t handler = 0; handler < HAL_SYSTICK_MAX_HOOKS; handler++ )
    {
        if( tick_hooks[handler].func != NULL )
        {
            if( ++tick_hooks[handler].ticks >= tick_hooks[handler].count )
            {
                ( tick_hooks[handler].func )();
                tick_hooks[handler].ticks = 0;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */

void SysTick_Handler( void )
{
    tick_timer++;
    hal_systick_callback();
}

/* ----- End ---------------------------------------------------------------- */
