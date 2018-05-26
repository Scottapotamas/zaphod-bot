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

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
HAL_TIM_MspPostInit( TIM_HandleTypeDef* htim );

/* ----- Public Functions --------------------------------------------------- */

void hal_pwm_setup_hlfb(uint8_t servo_number)
{
	TIM_HandleTypeDef* tim_handle = 0;

	switch (servo_number) {
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
	}

	// Setup
	//instance is set in above switch to reduce repetition
	tim_handle->Init.Prescaler 			= 0;
	tim_handle->Init.Period 			= 0;
	tim_handle->Init.CounterMode 		= TIM_COUNTERMODE_UP;
	tim_handle->Init.ClockDivision 		= TIM_CLOCKDIVISION_DIV1;
	tim_handle->Init.RepetitionCounter 	= 0;

	if (HAL_TIM_PWM_Init(tim_handle) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	if (HAL_TIM_IC_Init(tim_handle) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	// Setup
	TIM_SlaveConfigTypeDef sSlaveConfig;

	sSlaveConfig.SlaveMode 			= TIM_SLAVEMODE_RESET;
	sSlaveConfig.InputTrigger 		= TIM_TS_TI1FP1;
	sSlaveConfig.TriggerPolarity 	= TIM_INPUTCHANNELPOLARITY_RISING;
	sSlaveConfig.TriggerPrescaler 	= TIM_ICPSC_DIV1;
	sSlaveConfig.TriggerFilter 		= 0;

	if (HAL_TIM_SlaveConfigSynchronization(tim_handle, &sSlaveConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}


	TIM_IC_InitTypeDef sConfigIC;
	sConfigIC.ICPrescaler 			= TIM_ICPSC_DIV1;
	sConfigIC.ICFilter 				= 0;

	//Channel 1
	sConfigIC.ICPolarity 			= TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection 			= TIM_ICSELECTION_DIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(tim_handle, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	//Channel 2
	sConfigIC.ICPolarity 			= TIM_INPUTCHANNELPOLARITY_FALLING;
	sConfigIC.ICSelection 			= TIM_ICSELECTION_INDIRECTTI;
	if (HAL_TIM_IC_ConfigChannel(tim_handle, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}


	TIM_MasterConfigTypeDef sMasterConfig;

	sMasterConfig.MasterOutputTrigger 		= TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode 			= TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(tim_handle, &sMasterConfig) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}


	if(servo_number == _HLFB_SERVO_3 )
	{
		TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

		sBreakDeadTimeConfig.OffStateRunMode 	= TIM_OSSR_DISABLE;
		sBreakDeadTimeConfig.OffStateIDLEMode 	= TIM_OSSI_DISABLE;
		sBreakDeadTimeConfig.LockLevel 			= TIM_LOCKLEVEL_OFF;
		sBreakDeadTimeConfig.DeadTime 			= 0;
		sBreakDeadTimeConfig.BreakState 		= TIM_BREAK_DISABLE;
		sBreakDeadTimeConfig.BreakPolarity 		= TIM_BREAKPOLARITY_HIGH;
		sBreakDeadTimeConfig.AutomaticOutput 	= TIM_AUTOMATICOUTPUT_DISABLE;

		if (HAL_TIMEx_ConfigBreakDeadTime(tim_handle, &sBreakDeadTimeConfig) != HAL_OK)
		{
			_Error_Handler(__FILE__, __LINE__);
		}
	}

	HAL_TIM_MspPostInit(tim_handle);
}

void hal_pwm_setup_ic(void)
{
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 839;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 0;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_IC_Init(&htim9) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchronization(&htim9, &sSlaveConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void hal_pwm_setup(PWMOutputTimerDef_t pwm_output, uint16_t frequency, uint8_t duty_cycle)
{
	#define MAX_RELOAD               0xFFFF	//16-bit timer capability

	uint32_t period_cycles 	= ( SystemCoreClock / 2 ) / frequency;
	uint16_t prescaler 		= (uint16_t)(period_cycles / MAX_RELOAD + 1);
	uint16_t overflow 		= (uint16_t)((period_cycles + (prescaler / 2)) / prescaler);
	uint16_t duty_counts 	= overflow * duty_cycle / 100;	//duty cycle percentage of overflow

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

	tim_handle->Init.Prescaler 		= prescaler;
	tim_handle->Init.Period 		= overflow;
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
	sConfigOC.Pulse 				= duty_counts;
	sConfigOC.OCPolarity 			= TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode 			= TIM_OCFAST_DISABLE;

	//todo see if tim12 is happy having a full setup repeated for Ch2 output, or if Ch1 and Ch2 need to be done at same time.
	//aux2 is on ch1, everything else is on ch1
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

}

// TIM Group 4 - Input Capture
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{
    //enable peripheral clock
    if( tim_icHandle->Instance == TIM9 )
    {
    	__HAL_RCC_TIM9_CLK_ENABLE();
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
		hal_gpio_init_alternate( _BUZZER, GPIO_MODE_AF_PP, GPIO_AF3_TIM11, GPIO_SPEED_FREQ_LOW, GPIO_NOPULL );
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

/* ----- End ---------------------------------------------------------------- */
