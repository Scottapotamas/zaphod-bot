/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "hal_hard_ic.h"
#include "hal_gpio.h"
#include "average_short.h"
#include "qassert.h"

#include "stm32f4xx_hal.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/*
 * TACH    - TIM9_1
 * HLFB 1 - TIM3_1/2
 * HLFB 2 - TIM4_1/2
 * HLFB 3 - TIM1_1/2
 * HLFB 4 - TIM5_1/2
*/

/* ----- Variables ---------------------------------------------------------- */

//Timers allocated to servos (HLFB on CH1/2 as PWM input, A and B are ch 3, 4
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

// Fan Hall Input
TIM_HandleTypeDef htim9;

PRIVATE uint32_t       ic_values[HAL_HARD_IC_NUM];      // raw values per calculation
PRIVATE uint32_t       ic_peaks[HAL_HARD_IC_NUM];       // track the running highest value
PRIVATE AverageShort_t ic_averages[HAL_HARD_IC_NUM];    // simple average over a span

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
hal_setup_capture(uint8_t input);

PRIVATE void
HAL_TIM_MspPostInit( TIM_HandleTypeDef* htim );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
hal_hard_ic_init( void )
{
    memset( &ic_values,    0, sizeof( ic_values ) );
    memset( &ic_peaks,    0, sizeof( ic_peaks ) );
    memset( &ic_averages, 0, sizeof( ic_averages ) );

    average_short_init(&ic_averages[HAL_HARD_IC_FAN_HALL], 4 );
//    average_short_init( &ic_averages[HAL_HARD_IC_HLFB_SERVO_1],    4 );

    hal_setup_capture(HAL_HARD_IC_FAN_HALL );
}

PUBLIC bool
hal_hard_ic_valid( InputCaptureSignal_t input )
{
    REQUIRE(input < HAL_HARD_IC_NUM );
    return ic_averages[input].counter > 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_hard_ic_read( InputCaptureSignal_t input )
{
    REQUIRE(input < HAL_HARD_IC_NUM );
    return (uint32_t)average_short_get_last( &ic_averages[input] );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_hard_ic_read_avg( InputCaptureSignal_t input )
{
    REQUIRE(input < HAL_HARD_IC_NUM );
    return (uint32_t)average_short_get_average( &ic_averages[input] );
}

PUBLIC uint32_t
hal_hard_ic_read_peak( InputCaptureSignal_t input )
{
    REQUIRE(input < HAL_HARD_IC_NUM );
    return ic_peaks[input];
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_setup_capture(uint8_t input)
{
	TIM_HandleTypeDef* tim_handle = 0;

	switch (input)
	{
		case HAL_HARD_IC_HLFB_SERVO_1:
			tim_handle = &htim3;
			tim_handle->Instance = TIM3;
			break;

		case HAL_HARD_IC_HLFB_SERVO_2:
			tim_handle = &htim4;
			tim_handle->Instance = TIM4;
			break;

		case HAL_HARD_IC_HLFB_SERVO_3:
			tim_handle = &htim1;
			tim_handle->Instance = TIM1;
			break;

		case HAL_HARD_IC_HLFB_SERVO_4:
			tim_handle = &htim5;
			tim_handle->Instance = TIM5;
			break;

		case HAL_HARD_IC_FAN_HALL:
			tim_handle = &htim9;
			tim_handle->Instance = TIM9;
    }

	// Setup the Timer
    tim_handle->Init.Period 			= 0xFFFF;
    tim_handle->Init.Prescaler 			= 200;
    tim_handle->Init.ClockDivision 		= TIM_CLOCKDIVISION_DIV1;
    tim_handle->Init.CounterMode 		= TIM_COUNTERMODE_UP;
    tim_handle->Init.RepetitionCounter 	= 0;

    if (HAL_TIM_IC_Init(tim_handle) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    // Setup the Input Capture Channel
    TIM_IC_InitTypeDef sConfigIC;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;

    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    if (HAL_TIM_IC_ConfigChannel(tim_handle, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    // Start the Input capture in Interrupt mode
    HAL_TIM_IC_Start_IT(tim_handle, TIM_CHANNEL_1);

    HAL_TIM_MspPostInit(tim_handle);

}

/* -------------------------------------------------------------------------- */

// TIM Group 3 - PWM Input
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{
	if( tim_pwmHandle->Instance == TIM1 )
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		hal_gpio_init_alternate( _SERVO_3_HLFB, GPIO_MODE_AF_PP, GPIO_AF1_TIM1, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	if( tim_pwmHandle->Instance == TIM3 )
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
		hal_gpio_init_alternate( _SERVO_1_HLFB, GPIO_MODE_AF_PP, GPIO_AF2_TIM3, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	if( tim_pwmHandle->Instance == TIM4 )
	{
		__HAL_RCC_TIM4_CLK_ENABLE();
		hal_gpio_init_alternate( _SERVO_2_HLFB, GPIO_MODE_AF_PP, GPIO_AF2_TIM4, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	if( tim_pwmHandle->Instance == TIM5 )
	{
		__HAL_RCC_TIM5_CLK_ENABLE();
		hal_gpio_init_alternate( _SERVO_4_HLFB, GPIO_MODE_AF_PP, GPIO_AF2_TIM5, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

    if( tim_pwmHandle->Instance == TIM9 )
    {
    	__HAL_RCC_TIM9_CLK_ENABLE();
		hal_gpio_init_alternate(HAL_HARD_IC_FAN_HALL, GPIO_MODE_AF_PP, GPIO_AF3_TIM9, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
    }
}

// TIM Group 4 - Input Capture
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{

	if( tim_icHandle->Instance == TIM1 )
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		hal_gpio_init_alternate( _SERVO_3_HLFB, GPIO_MODE_AF_PP, GPIO_AF1_TIM1, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
		HAL_NVIC_SetPriority( TIM1_CC_IRQn, 4, 1 );
		HAL_NVIC_EnableIRQ( TIM1_CC_IRQn );
		//todo work out if the interrupt assignment here is correct
	}

	if( tim_icHandle->Instance == TIM3 )
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
		hal_gpio_init_alternate( _SERVO_1_HLFB, GPIO_MODE_AF_PP, GPIO_AF2_TIM3, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
		HAL_NVIC_SetPriority( TIM3_IRQn, 4, 2 );
		HAL_NVIC_EnableIRQ( TIM3_IRQn );
	}

	if( tim_icHandle->Instance == TIM4 )
	{
		__HAL_RCC_TIM4_CLK_ENABLE();
		hal_gpio_init_alternate( _SERVO_2_HLFB, GPIO_MODE_AF_PP, GPIO_AF2_TIM4, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
		HAL_NVIC_SetPriority( TIM4_IRQn, 4, 3 );
		HAL_NVIC_EnableIRQ( TIM4_IRQn );
	}

	if( tim_icHandle->Instance == TIM5 )
	{
		__HAL_RCC_TIM5_CLK_ENABLE();
		hal_gpio_init_alternate( _SERVO_4_HLFB, GPIO_MODE_AF_PP, GPIO_AF2_TIM5, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
		HAL_NVIC_SetPriority( TIM5_IRQn, 4, 4 );
		HAL_NVIC_EnableIRQ( TIM5_IRQn );
	}

	if( tim_icHandle->Instance == TIM9 )
    {
    	__HAL_RCC_TIM9_CLK_ENABLE();
		hal_gpio_init_alternate( _FAN_TACHO, GPIO_MODE_AF_PP, GPIO_AF3_TIM9, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
		HAL_NVIC_SetPriority( TIM1_BRK_TIM9_IRQn, 6, 3 );
		HAL_NVIC_EnableIRQ( TIM1_BRK_TIM9_IRQn );
    }

}

/* -------------------------------------------------------------------------- */

PRIVATE void
HAL_TIM_MspPostInit( TIM_HandleTypeDef* htim )
{
	if( htim->Instance==TIM9 )
	{
		hal_gpio_init_alternate( _FAN_TACHO, GPIO_MODE_AF_PP, GPIO_AF3_TIM9, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	//unused servo A and B pins, used for potential step/direction outputs in future
	/*
	  if(htim->Instance==TIM1)
	  {
	    GPIO_InitStruct.Pin = SERVO_3_A_Pin|SERVO_3_B_Pin;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	  }
	  else if(htim->Instance==TIM3)
	  {
	    GPIO_InitStruct.Pin = SERVO_1_A_Pin|SERVO_1_B_Pin;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	  }
	  else if(htim->Instance==TIM4)
	  {
	    GPIO_InitStruct.Pin = SERVO_2_A_Pin|SERVO_2_B_Pin;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	  }
	  else if(htim->Instance==TIM5)
	  {
	    GPIO_InitStruct.Pin = SERVO_4_A_Pin|SERVO_4_B_Pin;
	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull = GPIO_NOPULL;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  }
	 */

}

/* -------------------------------------------------------------------------- */

// TIM Group 3 - PWM Input
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{
	if(tim_pwmHandle->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_DISABLE();

		hal_gpio_disable_pin( _SERVO_3_HLFB );
		hal_gpio_disable_pin( _SERVO_3_A );
		hal_gpio_disable_pin( _SERVO_3_B );

	}
	else if(tim_pwmHandle->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_DISABLE();
	}
	else if(tim_pwmHandle->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_DISABLE();

		hal_gpio_disable_pin( _SERVO_1_HLFB );
		hal_gpio_disable_pin( _SERVO_1_A );
		hal_gpio_disable_pin( _SERVO_1_B );
	}
	else if(tim_pwmHandle->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_DISABLE();

		hal_gpio_disable_pin( _SERVO_2_HLFB );
		hal_gpio_disable_pin( _SERVO_2_A );
		hal_gpio_disable_pin( _SERVO_2_B );
	}
	else if(tim_pwmHandle->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_DISABLE();

		hal_gpio_disable_pin( _SERVO_4_HLFB );
		hal_gpio_disable_pin( _SERVO_4_A );
		hal_gpio_disable_pin( _SERVO_4_B );
	}
	else if(tim_pwmHandle->Instance==TIM12)
	{
		__HAL_RCC_TIM12_CLK_DISABLE();
	}
}

// TIM Group 4 - Input Capture
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{
	if(tim_icHandle->Instance==TIM9)
	{
		__HAL_RCC_TIM9_CLK_DISABLE();
		hal_gpio_disable_pin( _FAN_TACHO );
        HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);
    }
}

/* -------------------------------------------------------------------------- */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
	}
}


bool ic_capture_started;
uint32_t counts_a;
uint32_t counts_b;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint16_t capture_buffer = 0;

    //HLFB 1 - TIM3_1/2
    //HLFB 2 - TIM4_1/2
    //HLFB 3 - TIM1_1/2
    //HLFB 4 - TIM5_1/2
    //TACH   - TIM9_1

	if(htim->Instance==TIM1)
	{
//        HAL_HARD_IC_HLFB_SERVO_3
	}
	else if(htim->Instance==TIM3)
	{
//        HAL_HARD_IC_HLFB_SERVO_1
	}
	else if(htim->Instance==TIM4)
	{
//        HAL_HARD_IC_HLFB_SERVO_2
	}
	else if(htim->Instance==TIM5)
	{
//        HAL_HARD_IC_HLFB_SERVO_4
	}
	else if(htim->Instance==TIM9)
	{
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            capture_buffer = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

            // Catch the first edge to start our timing reference
            if( !ic_capture_started )
            {
                counts_a = capture_buffer;
                ic_capture_started = true;
            }
            else    // second edge is the other half of the square-wave
            {
                uint32_t delta_counts = 0;
                counts_b = capture_buffer;

                // Calculate the count delta between the two edges
                if (counts_b > counts_a) {
                    delta_counts = counts_b - counts_a;
                }
                else if (counts_b < counts_a)
                {
                    delta_counts = ((0xFFFF - counts_a) + counts_b) + 1;
                }

                ic_values[HAL_HARD_IC_FAN_HALL] = (HAL_RCC_GetPCLK2Freq() / delta_counts) / 100;
                average_short_update(&ic_averages[HAL_HARD_IC_FAN_HALL], (uint16_t)ic_values[HAL_HARD_IC_FAN_HALL] );
                ic_peaks[HAL_HARD_IC_FAN_HALL] = MAX(ic_peaks[HAL_HARD_IC_FAN_HALL], ic_values[HAL_HARD_IC_FAN_HALL] );

                ic_capture_started = false;   // reset to catch the next 'new' edge.
            }
        }
	}

}

/* ----- End ---------------------------------------------------------------- */
