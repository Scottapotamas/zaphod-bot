/* -------------------------------------------------------------------------- */
#include <stdarg.h>

#include "FreeRTOS.h"
#include "task.h"

#include "app_startup.h"

// TODO: move DWT related into a system/core HAL wrapper
#include "stm32f4xx_ll_cortex.h"

/* -------------------------------------------------------------------------- */

int main(void)
{
    app_startup_init();
    app_startup_tasks();

    vTaskStartScheduler();
    for (;;);

    return 0;
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