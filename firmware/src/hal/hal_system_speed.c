/* ----- System Includes ---------------------------------------------------- */

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rcc.h"

#include "hal_system_speed.h"

/* ----- Private Types ------------------------------------------------------ */

/* ----- Private Prototypes ------------------------------------------------- */

/* ----- Private Data ------------------------------------------------------- */

PRIVATE volatile uint32_t cc_when_sleeping;      //timestamp when we go to sleep
PRIVATE volatile uint32_t cc_when_woken  = 0;    //timestamp when we wake up
PRIVATE volatile uint32_t cc_awake_time  = 0;    //duration of 'active'
PRIVATE volatile uint32_t cc_asleep_time = 0;    //duration of 'sleep'

PRIVATE SystemSpeed_RCC_PLL_t pll_working;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
hal_system_speed_init( void )
{
    //Enable the DWT timer
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/* -------------------------------------------------------------------------- */

//Measured to take about 200us to execute

PUBLIC void
hal_system_speed_set_pll( SystemSpeed_RCC_PLL_t *pll_settings )
{
    uint16_t              timeout;
    SystemSpeed_RCC_PLL_t tmp;

    // Read the existing PLL registers
    hal_system_speed_get_pll( &tmp );

    if( memcmp( pll_settings, &tmp, sizeof( SystemSpeed_RCC_PLL_t ) ) == 0 )
    {
        return;    // Don't change PLL settings if settings are the same
    }

    //Enable the HSI48
    RCC->CR |= RCC_CR_HSION;

    // Ensure the HSI is ready
    timeout = 0xFFFF;
    while( !hal_system_speed_hsi_ready() && timeout-- ) {};

    // Use the HSI48 internal clock as main clock because the current PLL clock will stop.
    RCC->CFGR = ( RCC->CFGR & ~( RCC_CFGR_SW ) ) | RCC_CFGR_SW_HSI;

    //Disable the PLL
    RCC->CR &= ~RCC_CR_PLLON;

    // Set PLL settings from structure
    if( pll_settings->PLLM )
    {
        RCC->PLLCFGR = ( RCC->PLLCFGR & ~RCC_PLLM_MASK ) | ( ( pll_settings->PLLM << RCC_PLLM_POS ) & RCC_PLLM_MASK );
    }

    if( pll_settings->PLLN )
    {
        RCC->PLLCFGR = ( RCC->PLLCFGR & ~RCC_PLLN_MASK ) | ( ( pll_settings->PLLN << RCC_PLLN_POS ) & RCC_PLLN_MASK );
    }

    if( pll_settings->PLLP )
    {
        RCC->PLLCFGR = ( RCC->PLLCFGR & ~RCC_PLLP_MASK ) | ( ( ( ( pll_settings->PLLP >> 1 ) - 1 ) << RCC_PLLP_POS ) & RCC_PLLP_MASK );
    }

    if( pll_settings->PLLQ )
    {
        RCC->PLLCFGR = ( RCC->PLLCFGR & ~RCC_PLLQ_MASK ) | ( ( pll_settings->PLLQ << RCC_PLLQ_POS ) & RCC_PLLQ_MASK );
    }

    if( pll_settings->PLLR )
    {
        RCC->PLLCFGR = ( RCC->PLLCFGR & ~RCC_PLLR_MASK ) | ( ( pll_settings->PLLR << RCC_PLLR_POS ) & RCC_PLLR_MASK );
    }

    //Enable the PLL
    RCC->CR |= RCC_CR_PLLON;

    //Blocking wait until PLL is ready
    timeout = 0xFFFF;
    while( !hal_system_speed_pll_ready() && timeout-- ) {};

    //Enable PLL as main clock
    RCC->CFGR = ( RCC->CFGR & ~( RCC_CFGR_SW ) ) | RCC_CFGR_SW_PLL;

    //Update system core clock variable
    SystemCoreClockUpdate();
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_get_pll( SystemSpeed_RCC_PLL_t *pll_settings )
{
    pll_settings->PLLM = ( RCC->PLLCFGR & RCC_PLLM_MASK ) >> RCC_PLLM_POS;
    pll_settings->PLLN = ( RCC->PLLCFGR & RCC_PLLN_MASK ) >> RCC_PLLN_POS;
    pll_settings->PLLP = ( ( ( RCC->PLLCFGR & RCC_PLLP_MASK ) >> RCC_PLLP_POS ) + 1 ) << 1;
    pll_settings->PLLQ = ( RCC->PLLCFGR & RCC_PLLQ_MASK ) >> RCC_PLLQ_POS;
    pll_settings->PLLR = ( RCC->PLLCFGR & RCC_PLLR_MASK ) >> RCC_PLLR_POS;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
hal_system_speed_pll_ready( void )
{
    return ( RCC->CR & RCC_CR_PLLRDY );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
hal_system_speed_hsi_ready( void )
{
    return ( RCC->CR & RCC_CR_HSIRDY );
}
/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_sleep( void )
{
    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    cc_awake_time += DWT->CYCCNT - cc_when_woken;
    cc_when_sleeping = DWT->CYCCNT;

    //Go to sleep. Wake on interrupt.
    LL_LPM_EnableSleep();
    __WFI();

    cc_asleep_time += DWT->CYCCNT - cc_when_sleeping;
    cc_when_woken = DWT->CYCCNT;

    CRITICAL_SECTION_END();
}

/* -------------------------------------------------------------------------- */

//return a 0-100% value for CPU load
PUBLIC float
hal_system_speed_get_load( void )
{
    float cpu_util = (float)cc_awake_time
                     / (float)( cc_asleep_time + cc_awake_time ) * 100.0f;

    cc_asleep_time = 0;
    cc_awake_time  = 0;

    return cpu_util;
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_system_speed_get_speed( void )
{
    LL_RCC_ClocksTypeDef rcc_clks = { 0 };
    LL_RCC_GetSystemClocksFreq( &rcc_clks );

    return rcc_clks.HCLK_Frequency;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_high( void )
{
    hal_system_speed_get_pll( &pll_working );

    pll_working.PLLN = 336; /* 168MHz System clock */
    pll_working.PLLP = 2;
    pll_working.PLLQ = 7; /* 48MHz Clock for USB */

    hal_system_speed_set_pll( &pll_working );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_system_speed_low( void )
{
    hal_system_speed_get_pll( &pll_working );

    pll_working.PLLN = 144; /* 72MHz System Clock */
    pll_working.PLLP = 2;
    pll_working.PLLQ = 3; /* 48MHz Clock for USB */

    hal_system_speed_set_pll( &pll_working );
}

/* ----- End ---------------------------------------------------------------- */
