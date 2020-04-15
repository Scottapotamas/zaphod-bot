/* ----- Local Includes ----------------------------------------------------- */

#include "hal_watchdog.h"
#include "stm32f4xx_ll_iwdg.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_watchdog_init( uint32_t timeout_ms )
{
    LL_IWDG_Enable( IWDG );
    LL_IWDG_EnableWriteAccess( IWDG );
    LL_IWDG_SetPrescaler( IWDG, LL_IWDG_PRESCALER_256 );
    LL_IWDG_SetReloadCounter( IWDG, MAX( 1UL, MIN( 4095UL, timeout_ms / 8UL ) ) );

    while( LL_IWDG_IsReady( IWDG ) != 1 )
    {
    }
}

/* -------------------------------------------------------------------------- */

/** Kick the dog. */

PUBLIC void
hal_watchdog_refresh( void )
{
    LL_IWDG_ReloadCounter( IWDG );
}

/* ----- End ---------------------------------------------------------------- */
