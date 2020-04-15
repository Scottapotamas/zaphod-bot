/* ----- Local Includes ----------------------------------------------------- */

#include "hal_reset.h"
#include "stm32f4xx_ll_rcc.h"

#include "hal_delay.h"
#include "hal_watchdog.h"

/* -------------------------------------------------------------------------- */

PUBLIC HalPowerResetCause_t
hal_reset_cause( void )
{
    HalPowerResetCause_t cause = RESET_CAUSE_UNKNOWN;

    if( LL_RCC_IsActiveFlag_BORRST() )
    {
        cause = RESET_CAUSE_BOR;
    }
    else if( LL_RCC_IsActiveFlag_SFTRST() )
    {
        cause = RESET_CAUSE_SW;
    }
    else if( LL_RCC_IsActiveFlag_PORRST() )
    {
        cause = RESET_CAUSE_POR;
    }
    else if( LL_RCC_IsActiveFlag_PINRST() )
    {
        cause = RESET_CAUSE_PIN;
    }
    else if( LL_RCC_IsActiveFlag_LPWRRST() )
    {
        cause = RESET_CAUSE_LPWR;
    }
    else if( LL_RCC_IsActiveFlag_IWDGRST() )
    {
        cause = RESET_CAUSE_IWDG;
    }
    else if( LL_RCC_IsActiveFlag_WWDGRST() )
    {
        cause = RESET_CAUSE_WWDG;
    }

    LL_RCC_ClearResetFlags();

    return cause;
}

/* -------------------------------------------------------------------------- */

PUBLIC const char *
hal_reset_cause_description( HalPowerResetCause_t cause )
{
    switch( cause )
    {
        case RESET_CAUSE_BOR:
            return "Brown Out";
        case RESET_CAUSE_PIN:
            return "Reset Pin";
        case RESET_CAUSE_POR:
            return "Power On";
        case RESET_CAUSE_SW:
            return "Software Reset";
        case RESET_CAUSE_IWDG:
            return "Independent Watchdog";
        case RESET_CAUSE_WWDG:
            return "Window Watchdog";
        case RESET_CAUSE_LPWR:
            return "Low Power";
        default:
            return "Unknown Reset Cause";
    }
}

/* -------------------------------------------------------------------------- */

// Trigger a reboot of the microcontroller core
PUBLIC void
hal_reset_software( void )
{
    hal_delay_ms( 10 );    // allow some time for serial buffers to flush etc
    hal_watchdog_refresh();
    NVIC_SystemReset();
}

/* ----- End ---------------------------------------------------------------- */
