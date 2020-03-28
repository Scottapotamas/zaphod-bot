/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_utils.h"

#include "global.h"
#include "qassert.h"
#include "app_tasks.h"
#include "app_hardware.h"
#include "status.h"
#include "hal_watchdog.h"
#include "hal_system_speed.h"

/* Assert printout requirements */
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "hal_delay.h"

/* Private variables ---------------------------------------------------------*/
#if defined(NASSERT) || defined(NDEBUG)
	#if defined( USE_FULL_ASSERT ) || (USE_FULL_ASSERT > 0)
		#error "You are building a RELEASE version while STM HAL library ASSERTs are still enabled!"
	#endif
#endif

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private function prototypes -----------------------------------------------*/

//application entry point from startup_stm32f429xx.s
int main(void)
{
	HAL_Init();             //Reset peripherals, init flash etc
	SystemClock_Config();   //Setup the system clock
	app_hardware_init();    //Initialise IO, peripherals etc

	app_tasks_init();       //start the task handler

	//allow interrupts
	PERMIT();

    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(1)
	{
		if( !app_tasks_run() )
		{
			status_red(false);
			hal_watchdog_refresh();
			hal_system_speed_sleep();
		}
		hal_watchdog_refresh();
		status_red(true);
	}
}

// Startup the internal and external clocks, set PLL etc
void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

    if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5)
    {
        Error_Handler();
    }

    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_PWR_DisableOverDriveMode();
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while(LL_RCC_HSE_IsReady() != 1)
    {

    }
    LL_RCC_LSI_Enable();

    /* Wait till LSI is ready */
    while(LL_RCC_LSI_IsReady() != 1)
    {

    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_4, 168, LL_RCC_PLLQ_DIV_7);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while(LL_RCC_PLL_IsReady() != 1)
    {

    }

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_4);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {

    }
    LL_SetSystemCoreClock(168000000);

    /* Update the time base */
    if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
    {
        Error_Handler();
    };

    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);



    // Configure the Systick interrupt time
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

    // Configure the Systick
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    // SysTick_IRQn interrupt configuration
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void onAssert__( const char * file,
                 unsigned     line,
                 const char * fmt,
                 ... )
{
    va_list args;
    char message[32];
    int len;
    memset( message, 0, sizeof(message) );

    // Format the printf part
    if( fmt && ( strlen( fmt ) > 0 ) )
    {
        va_start( args, fmt );
        len = vsnprintf( message, sizeof(message), fmt, args );
        va_end( args );
    }

    // Blinking lights while we wait for the watch dog to bite
    status_red( true );
    status_yellow( true );
    status_green( false );

    while (1)
    {
        hal_delay_ms( 100 );
        status_red_toggle();
        status_green_toggle();
    }
}

void _Error_Handler(char *file, int line)
{
  while(1)
  {

  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
  // TODO: implement full assert handler
}
#endif /* USE_FULL_ASSERT */
