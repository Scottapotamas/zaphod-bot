/* -------------------------------------------------------------------------- */
#include <stdarg.h>

#include <stm32f4xx_ll_rcc.h>
#include <stm32f4xx_ll_system.h>
#include <stm32f4xx_ll_bus.h>
#include <stm32f4xx_ll_pwr.h>
#include <stm32f4xx_ll_utils.h>

#include "hal_gpio.h"

#include "FreeRTOS.h"
#include "task.h"

/* -------------------------------------------------------------------------- */

static void init_core( void );
static void system_clock_config( void );

/* -------------------------------------------------------------------------- */


static void blinkGreenTask(void *arg)
{
    for(;;)
    {
        vTaskDelay(700);
        hal_gpio_toggle_pin( _STATUS_0 );
    }
}

static void blinkRedTask(void *arg)
{
    for(;;)
    {
        vTaskDelay(150);
        hal_gpio_toggle_pin( _STATUS_1 );
    }
}

/* -------------------------------------------------------------------------- */

int main(void)
{
    init_core();
    system_clock_config();

    hal_gpio_configure_defaults();


    xTaskCreate(blinkGreenTask, "blinkG", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(blinkRedTask, "blinkR", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();
    for (;;);

    return 0;
}

/* -------------------------------------------------------------------------- */

#define NVIC_PRIORITYGROUP_4 ( (uint32_t)0x00000003 ) /*!< 4 bits for pre-emption priority,
                                                           0 bits for subpriority */

static void init_core( void )
{
    LL_FLASH_EnableInstCache();
    LL_FLASH_EnableDataCache();
    LL_FLASH_EnablePrefetch();

    LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_SYSCFG );
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );

    NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );

    NVIC_SetPriority( MemoryManagement_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( BusFault_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( UsageFault_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( SVCall_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( DebugMonitor_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( PendSV_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_SetPriority( SysTick_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 0, 0 ) );
}

// Startup the internal and external clocks, set PLL etc
static void system_clock_config( void )
{
    LL_FLASH_SetLatency( LL_FLASH_LATENCY_5 );

    if( LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5 )
    {
        configASSERT(0);
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

void vApplicationTickHook(void)
{

}

void vApplicationIdleHook(void)
{
    // A reasonable place to feed the watchdog if no other critical task makes sense

#ifndef configUSE_TICKLESS_IDLE
    // Put micro into lower power state.
    __WFI();
#endif
}

void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for(;;);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    taskDISABLE_INTERRUPTS();
    for(;;);
}

/* -------------------------------------------------------------------------- */

void portAssertHandler( const char *file,
                        unsigned    line,
                        const char *fmt,
                        ... )
{
    va_list  args;

    // Forward directly to the 'in-memory cache' handler function
    va_start( args, fmt );
    //    cache_assert_reason_in_ram( file, line, fmt, args );
    va_end( args );

    // Wait for the watch dog to bite
    for( ;; )
    {
        asm("NOP");
    }

}

/* -------------------------------------------------------------------------- */

void portConfigureTimerForRuntimeStats(void)
{
    //Enable the DWT timer
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t portGetRuntimeCounterValue(void)
{
    // DWT timer value
    return DWT->CYCCNT;
}

/* -------------------------------------------------------------------------- */