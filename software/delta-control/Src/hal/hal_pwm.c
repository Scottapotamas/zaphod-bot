/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "hal_pwm.h"
#include "hal_gpio.h"

#include "stm32f4xx_hal.h"

/* ----- Defines ------------------------------------------------------------ */

#define FAN_PRESCALER		0
#define FAN_PWM_PERIOD 		3359

#define BUZZER_PRESCALER	1
#define BUZZER_PWM_PERIOD	16800

#define AUX_PRESCALER		0
#define AUX_PWM_PERIOD		4800

/*
 * HLFB 1 - TIM3_1/2
 * HLFB 2 - TIM4_1/2
 * HLFB 3 - TIM1_1/2
 * HLFB 4 - TIM5_1/2
 *
 * A and B are on same TIM as HLFB, but not planned for use right now
 *
 * TACH    - TIM9_1
 * FAN     - TIM10_1
 * BUZZER  - TIM11_1
 * AUX0    - TIM2_1
 * AUX1    - TIM12_2
 * AUX2    - TIM12_1
 */


/* ----- Variables ---------------------------------------------------------- */

//Timers allocated to servos (HLFB on CH1/2 as PWM input, A and B are ch 3, 4
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

//Fan
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim10;

//Buzzer
TIM_HandleTypeDef htim11;

// Internal Aux PWM
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim12;

PWMInputData_t input_results[ _HLFB_SERVO_NUM + 1 ];	//todo use updated inputs enum after refactor

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
HAL_TIM_MspPostInit( TIM_HandleTypeDef* htim );

/* ----- Public Functions --------------------------------------------------- */

void hal_setup_capture(uint8_t input)
{

	TIM_HandleTypeDef* tim_handle = 0;

	switch (input)
	{
		case _HLFB_SERVO_1:
			tim_handle = &htim3;
			tim_handle->Instance = TIM3;
			break;

		case _HLFB_SERVO_2:
			tim_handle = &htim4;
			tim_handle->Instance = TIM4;
			break;

		case _HLFB_SERVO_3:
			tim_handle = &htim1;
			tim_handle->Instance = TIM1;
			break;

		case _HLFB_SERVO_4:
			tim_handle = &htim5;
			tim_handle->Instance = TIM5;
			break;

		case _FAN_HALL:
			tim_handle = &htim9;
			tim_handle->Instance = TIM9;
	}

	// Setup
	tim_handle->Init.Prescaler 			= 839;
	tim_handle->Init.Period 			= 0xFFFF;
	tim_handle->Init.CounterMode 		= TIM_COUNTERMODE_UP;
	tim_handle->Init.ClockDivision 		= TIM_CLOCKDIVISION_DIV1;
	tim_handle->Init.RepetitionCounter 	= 0;

	  if (HAL_TIM_Base_Init(tim_handle) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  TIM_ClockConfigTypeDef sClockSourceConfig;

	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(tim_handle, &sClockSourceConfig) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  if (HAL_TIM_IC_Init(tim_handle) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  TIM_MasterConfigTypeDef sMasterConfig;

	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(tim_handle, &sMasterConfig) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }

	  TIM_IC_InitTypeDef sConfigIC;

	  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	  sConfigIC.ICFilter = 0;
	  if (HAL_TIM_IC_ConfigChannel(tim_handle, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }




	  hal_gpio_init_alternate(_FAN_TACHO, MODE_AF_PP, GPIO_AF3_TIM9, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL);

	  HAL_NVIC_SetPriority( TIM1_BRK_TIM9_IRQn, 4, 3 );
	  HAL_NVIC_EnableIRQ( TIM1_BRK_TIM9_IRQn );

	  HAL_TIM_Base_Start(tim_handle);
	  HAL_TIM_IC_Start_IT(tim_handle, TIM_CHANNEL_1);

}

/* -------------------------------------------------------------------------- */

void hal_pwm_generation(PWMOutputTimerDef_t pwm_output, uint16_t frequency)
{
	TIM_HandleTypeDef* tim_handle = 0;

	switch(pwm_output)
	{
		case _PWM_TIM_FAN:
			tim_handle = &htim10;
			tim_handle->Instance = TIM10;
			break;

		case _PWM_TIM_BUZZER:
			tim_handle = &htim11;
			tim_handle->Instance = TIM11;
			break;

		case _PWM_TIM_AUX_0:
			tim_handle = &htim2;
			tim_handle->Instance = TIM2;
			break;

		case _PWM_TIM_AUX_1:
			tim_handle = &htim12;
			tim_handle->Instance = TIM12;
			break;

		case _PWM_TIM_AUX_2:
			tim_handle = &htim12;
			tim_handle->Instance = TIM12;
			break;
		default:

			break;
	}

//    #define MAX_RELOAD               0xFFFF	//16-bit timer capability
    #define PWM_PERIOD_DEFAULT  1024

//    uint32_t period_cycles 	= ( SystemCoreClock / 2 ) / frequency;
//    uint16_t prescaler 		= (uint16_t)(period_cycles / MAX_RELOAD + 1);
//    uint16_t overflow 		= (uint16_t)((period_cycles + (prescaler / 2)) / prescaler);
//    uint16_t duty_counts 	= overflow * duty_cycle / 100;	//duty cycle percentage of overflow

    tim_handle->Init.Prescaler 		= (uint32_t)( SystemCoreClock / ( frequency * PWM_PERIOD_DEFAULT ) );//prescaler;
	tim_handle->Init.Period 		= PWM_PERIOD_DEFAULT; //overflow;
	tim_handle->Init.CounterMode 	= TIM_COUNTERMODE_UP;
	tim_handle->Init.ClockDivision 	= TIM_CLOCKDIVISION_DIV1;

	//no base for AUX outputs (according to cubeMX export)?
	if( pwm_output == _PWM_TIM_FAN || pwm_output == _PWM_TIM_BUZZER )
	{
		if (HAL_TIM_Base_Init(tim_handle) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
	}

//    TIM_ClockConfigTypeDef sClockSourceConfig;
//    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//    HAL_TIM_ConfigClockSource( tim_handle, &sClockSourceConfig );

	if (HAL_TIM_PWM_Init(tim_handle) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	//TIM2 has master config according to cubemx output
	if( pwm_output == _PWM_TIM_AUX_0)
	{
		TIM_MasterConfigTypeDef sMasterConfig;

		sMasterConfig.MasterOutputTrigger 	= TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode 		= TIM_MASTERSLAVEMODE_DISABLE;

		if (HAL_TIMEx_MasterConfigSynchronization(tim_handle, &sMasterConfig) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
	}

	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode 				= TIM_OCMODE_PWM1;
	sConfigOC.Pulse 				= 1;
	sConfigOC.OCPolarity 			= TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode 			= TIM_OCFAST_DISABLE;

	//todo see if tim12 is happy having a full setup repeated for Ch2 output, or if Ch1 and Ch2 need to be done at same time.
	//aux2 is on ch2, everything else is on ch1
	if( pwm_output == _PWM_TIM_AUX_2)
	{
		if (HAL_TIM_PWM_ConfigChannel(tim_handle, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
		HAL_TIM_PWM_Start(tim_handle,TIM_CHANNEL_2);
	}
	else
	{
		if (HAL_TIM_PWM_ConfigChannel(tim_handle, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
		HAL_TIM_PWM_Start(tim_handle,TIM_CHANNEL_1);
	}

	HAL_TIM_MspPostInit(tim_handle);
}

/* -------------------------------------------------------------------------- */

// O-100% as a float
PUBLIC void hal_pwm_set_percentage_f( PWMOutputTimerDef_t pwm_output, float percentage )
{
    if(percentage >= 0.000 && percentage <= 100.000)
    {
        uint16_t duty_as_u16 = (percentage/100.0)*0xFFFFU;
        hal_pwm_set( pwm_output, duty_as_u16 );
    }
    else
    {
        hal_pwm_set( pwm_output, 0 );
    }
}

// 0-255 byte as 0-full duty cycle helper function
PUBLIC void hal_pwm_set_byte( PWMOutputTimerDef_t pwm_output, uint8_t duty_cycle )
{
    uint16_t duty_as_u16 = duty_cycle*0xFFU;
    hal_pwm_set( pwm_output, duty_as_u16 );
}

PUBLIC void hal_pwm_set(PWMOutputTimerDef_t pwm_output, uint16_t duty_cycle)
{
	TIM_HandleTypeDef* tim_handle = 0;
    uint32_t channel = 0;

	switch(pwm_output)
	{
		case _PWM_TIM_FAN:
			tim_handle = &htim10;
			tim_handle->Instance = TIM10;
            channel = TIM_CHANNEL_1;
			break;

		case _PWM_TIM_BUZZER:
			tim_handle = &htim11;
			tim_handle->Instance = TIM11;
            channel = TIM_CHANNEL_1;
			break;

		case _PWM_TIM_AUX_0:
			tim_handle = &htim2;
			tim_handle->Instance = TIM2;
            channel = TIM_CHANNEL_1;
            break;

		case _PWM_TIM_AUX_1:
			tim_handle = &htim12;
			tim_handle->Instance = TIM12;
            channel = TIM_CHANNEL_1;
			break;

		case _PWM_TIM_AUX_2:
			tim_handle = &htim12;
			tim_handle->Instance = TIM12;
            channel = TIM_CHANNEL_2;
			break;
		default:

			break;
	}

	//todo make this work properly

    uint32_t period = tim_handle->Init.Period;  //get

    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode 				= TIM_OCMODE_PWM1;
    sConfigOC.Pulse 				= ( MIN( duty_cycle, 0xFFFFU ) * period ) / 0xFFFFU;
    sConfigOC.OCPolarity 			= TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode 			= TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(tim_handle, &sConfigOC, channel);
    HAL_TIM_PWM_Start(tim_handle, channel);
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

	if( tim_pwmHandle->Instance == TIM2 )
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
	}

	if( tim_pwmHandle->Instance == TIM12 )
	{
		__HAL_RCC_TIM12_CLK_ENABLE();
	}

    if( tim_pwmHandle->Instance == TIM9 )
    {
    	__HAL_RCC_TIM9_CLK_ENABLE();
		hal_gpio_init_alternate( _FAN_HALL, GPIO_MODE_AF_PP, GPIO_AF3_TIM9, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
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
		hal_gpio_init_alternate( _FAN_HALL, GPIO_MODE_AF_PP, GPIO_AF3_TIM9, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
		HAL_NVIC_SetPriority( TIM1_BRK_TIM9_IRQn, 6, 3 );
		HAL_NVIC_EnableIRQ( TIM1_BRK_TIM9_IRQn );
    }

}

//TIM Base Functions
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
	if( tim_baseHandle->Instance == TIM10 )
	{
		__HAL_RCC_TIM10_CLK_ENABLE();
		hal_gpio_init_alternate( _FAN_PWM, GPIO_MODE_AF_PP, GPIO_AF3_TIM10, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	if( tim_baseHandle->Instance == TIM11 )
	{
		__HAL_RCC_TIM11_CLK_ENABLE();
		hal_gpio_init_alternate( _BUZZER, GPIO_MODE_AF_PP, GPIO_AF3_TIM11, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}
}


/* -------------------------------------------------------------------------- */


PRIVATE void
HAL_TIM_MspPostInit( TIM_HandleTypeDef* htim )
{
	if( htim->Instance == TIM10 )
	{
		hal_gpio_init_alternate( _FAN_PWM, GPIO_MODE_AF_PP, GPIO_AF3_TIM10, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	if( htim->Instance == TIM11 )
	{
//		hal_gpio_init_alternate( _BUZZER, GPIO_MODE_AF_PP, GPIO_AF3_TIM11, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	if( htim->Instance == TIM2 )
	{
		hal_gpio_init_alternate( _AUX_PWM_0, GPIO_MODE_AF_PP, GPIO_AF1_TIM2, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	if( htim->Instance==TIM12 )
	{
		hal_gpio_init_alternate( _AUX_PWM_1, GPIO_MODE_AF_PP, GPIO_AF9_TIM12, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
		hal_gpio_init_alternate( _AUX_PWM_2, GPIO_MODE_AF_PP, GPIO_AF9_TIM12, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
	}

	if( htim->Instance==TIM9 )
	{
		hal_gpio_init_alternate( _FAN_HALL, GPIO_MODE_AF_PP, GPIO_AF3_TIM9, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
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

PUBLIC void
HAL_TIM_Base_MspDeInit( TIM_HandleTypeDef* tim_baseHandle )
{
	if(tim_baseHandle->Instance==TIM10)
	{
		__HAL_RCC_TIM10_CLK_DISABLE();

	}
	else if(tim_baseHandle->Instance==TIM11)
	{
		__HAL_RCC_TIM11_CLK_DISABLE();

	}
}

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

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint16_t capture_buffer = 0;
	PWMInputData_t * result = 0;

	/*
	 * HLFB 1 - TIM3_1/2
	 * HLFB 2 - TIM4_1/2
	 * HLFB 3 - TIM1_1/2
	 * HLFB 4 - TIM5_1/2
	 * TACH   - TIM9_1
	 */

	if(htim->Instance==TIM1)
	{
		result = &input_results[_HLFB_SERVO_3];
	}
	else if(htim->Instance==TIM3)
	{
		result = &input_results[_HLFB_SERVO_1];
	}
	else if(htim->Instance==TIM4)
	{
		result = &input_results[_HLFB_SERVO_2];
	}
	else if(htim->Instance==TIM5)
	{
		result = &input_results[_HLFB_SERVO_4];
	}
	else if(htim->Instance==TIM9)
	{
		result = &input_results[_FAN_HALL];
	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{

	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		capture_buffer = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

		if (capture_buffer != 0)
		{
			result->duty 		= ( HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) * 100 ) / capture_buffer;
			result->frequency 	= ( HAL_RCC_GetHCLKFreq() / 2 ) / capture_buffer;
		}
		else
		{
			result->duty 		= 0;
			result->frequency 	= 0;
		}
	}
}
/* ----- End ---------------------------------------------------------------- */
