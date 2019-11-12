/* ----- Local Includes ----------------------------------------------------- */

#include "hal_watchdog.h"
#include "stm32f4xx_hal.h"

/* -------------------------------------------------------------------------- */

IWDG_HandleTypeDef hiwdg;

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_watchdog_init( uint32_t timeout_ms )
{
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_256; /* 125Hz or 8ms ticks */
    hiwdg.Init.Reload = MAX( 1UL, MIN( 4095UL, timeout_ms / 8UL ) );
    HAL_IWDG_Init( &hiwdg );
}

/* -------------------------------------------------------------------------- */

/** Kick the dog. */

PUBLIC void
hal_watchdog_refresh( void )
{
    HAL_IWDG_Refresh( &hiwdg );
}

/* ----- End ---------------------------------------------------------------- */
