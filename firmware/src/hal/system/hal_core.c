/* -------------------------------------------------------------------------- */

#include "hal_core.h"
#include "qassert.h"

#include <stm32f4xx_ll_rcc.h>
#include <stm32f4xx_ll_system.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_pwr.h>
#include <stm32f4xx_ll_utils.h>

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

#define NVIC_PRIORITYGROUP_4 ( (uint32_t)0x00000003 ) /*!< 4 bits for pre-emption priority,
                                                           0 bits for subpriority */

/* -------------------------------------------------------------------------- */

PUBLIC void hal_core_init( void )
{
    LL_FLASH_EnableInstCache();
    LL_FLASH_EnableDataCache();
    LL_FLASH_EnablePrefetch();

    LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_SYSCFG );
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );

    // We want all preempt priority bits and no subpriority bits
    //  - see discussion in https://www.freertos.org/RTOS-Cortex-M3-M4.html
    NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );

    NVIC_SetPriority( MemoryManagement_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( BusFault_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( UsageFault_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( SVCall_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( DebugMonitor_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( PendSV_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( SysTick_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
}

/* -------------------------------------------------------------------------- */

// Startup the internal and external clocks, set PLL etc
PUBLIC void hal_core_clock_configure( void )
{
    LL_FLASH_SetLatency( LL_FLASH_LATENCY_5 );

    if( LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5 )
    {
        ASSERT(false);
    }

    LL_PWR_SetRegulVoltageScaling( LL_PWR_REGU_VOLTAGE_SCALE1 );
    LL_PWR_DisableOverDriveMode();

    LL_RCC_HSE_Enable();
    while( LL_RCC_HSE_IsReady() != 1 )
    {
    }

    LL_RCC_LSI_Enable();
    while( LL_RCC_LSI_IsReady() != 1 )
    {
    }

    LL_RCC_PLL_ConfigDomain_SYS( LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2 );
    LL_RCC_PLL_Enable();
    while( LL_RCC_PLL_IsReady() != 1 )
    {
    }

    LL_RCC_SetAHBPrescaler( LL_RCC_SYSCLK_DIV_1 );
    LL_RCC_SetAPB1Prescaler( LL_RCC_APB1_DIV_4 );
    LL_RCC_SetAPB2Prescaler( LL_RCC_APB2_DIV_4 );

    LL_RCC_SetSysClkSource( LL_RCC_SYS_CLKSOURCE_PLL );
    while( LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL )
    {
    }

    LL_Init1msTick( 168000000 );
    LL_SetSystemCoreClock( 168000000 );
    LL_RCC_SetTIMPrescaler( LL_RCC_TIM_PRESCALER_TWICE );
}

/* -------------------------------------------------------------------------- */
