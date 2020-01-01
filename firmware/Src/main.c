/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "usb_device.h"

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

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
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

    /* Format the printf part */
    if( fmt && ( strlen( fmt ) > 0 ) )
    {
        va_start( args, fmt );
        len = vsnprintf( message, sizeof(message), fmt, args );
        va_end( args );
    }

	/* Show assert in terminal  */
    //hal_uart_printf_direct( HAL_UART_PORT_MAIN, "ASSERT: %s, line %d (%s)\r\n\r\n", file, line, message );

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

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
