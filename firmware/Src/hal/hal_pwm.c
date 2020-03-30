/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

#include "hal_pwm.h"
#include "hal_gpio.h"
#include "qassert.h"

#include "stm32f4xx_hal.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

#define PWM_PERIOD_DEFAULT  1024

#define FAN_PRESCALER		0
#define FAN_PWM_PERIOD 		3359

#define BUZZER_PRESCALER	1
#define BUZZER_PWM_PERIOD	16800

#define AUX_PRESCALER		0
#define AUX_PWM_PERIOD		4800

/*
 * TACH    - TIM9_1
 * HLFB 1 - TIM3_1/2
 * HLFB 2 - TIM4_1/2
 * HLFB 3 - TIM1_1/2
 * HLFB 4 - TIM5_1/2
 *
 * A and B are on same TIM as HLFB, but not planned for use right now
 *
 * FAN     - TIM10_1
 * BUZZER  - TIM11_1
 * AUX0    - TIM2_1
 * AUX1    - TIM12_2
 * AUX2    - TIM12_1
 */


/* ----- Variables ---------------------------------------------------------- */

// Fan Output
TIM_HandleTypeDef htim10;

// Buzzer
TIM_HandleTypeDef htim11;

// Internal Aux PWM outputs
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim12;

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
HAL_TIM_MspPostInit( TIM_HandleTypeDef* htim );

/* ----- Public Functions --------------------------------------------------- */

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

    tim_handle->Init.Prescaler 		= (uint32_t)( SystemCoreClock / ( frequency * PWM_PERIOD_DEFAULT ) ); //prescaler
	tim_handle->Init.Period 		= PWM_PERIOD_DEFAULT; //overflow;
	tim_handle->Init.CounterMode 	= TIM_COUNTERMODE_UP;
	tim_handle->Init.ClockDivision 	= TIM_CLOCKDIVISION_DIV1;

	//no base for AUX outputs (according to cubeMX export)?
	if( pwm_output == _PWM_TIM_FAN || pwm_output == _PWM_TIM_BUZZER )
	{
		if (HAL_TIM_Base_Init(tim_handle) != HAL_OK)
		{
			ASSERT( false );
		}
	}

	if (HAL_TIM_PWM_Init(tim_handle) != HAL_OK)
	{
		ASSERT( false );
	}

	//TIM2 has master config according to cubemx output
	if( pwm_output == _PWM_TIM_AUX_0)
	{
		TIM_MasterConfigTypeDef sMasterConfig;

		sMasterConfig.MasterOutputTrigger 	= TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode 		= TIM_MASTERSLAVEMODE_DISABLE;

		if (HAL_TIMEx_MasterConfigSynchronization(tim_handle, &sMasterConfig) != HAL_OK)
		{
			ASSERT( false );
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
			ASSERT( false );
		}
		HAL_TIM_PWM_Start(tim_handle,TIM_CHANNEL_2);
	}
	else
	{
		if (HAL_TIM_PWM_ConfigChannel(tim_handle, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
		{
			ASSERT( false );
		}
		HAL_TIM_PWM_Start(tim_handle,TIM_CHANNEL_1);
	}

	// Set the GPIO configurations to suit AF as needed
	HAL_TIM_MspPostInit(tim_handle);
}

/* -------------------------------------------------------------------------- */

// O-100% as a float
PUBLIC void hal_pwm_set_percentage_f( PWMOutputTimerDef_t pwm_output, float percentage )
{
    if(percentage >= 0.0f && percentage <= 100.0f)
    {
        uint16_t duty_as_u16 = (percentage/100.0f)*0xFFFFU;
        hal_pwm_set( pwm_output, duty_as_u16 );
    }
    else
    {
        hal_pwm_set( pwm_output, 0 );
    }
}

/* -------------------------------------------------------------------------- */

// 0-255 byte as 0-full duty cycle helper function
PUBLIC void hal_pwm_set_byte( PWMOutputTimerDef_t pwm_output, uint8_t duty_cycle )
{
    uint16_t duty_as_u16 = duty_cycle*0xFFU;
    hal_pwm_set( pwm_output, duty_as_u16 );
}

/* -------------------------------------------------------------------------- */

PRIVATE void hal_pwm_set(PWMOutputTimerDef_t pwm_output, uint16_t duty_cycle)
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

    uint32_t period = tim_handle->Init.Period;  //get

    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode 				= TIM_OCMODE_PWM1;
    sConfigOC.Pulse 				= ( MIN( duty_cycle, 0xFFFFU ) * period ) / 0xFFFFU;
    sConfigOC.OCPolarity 			= TIM_OCPOLARITY_LOW;
    sConfigOC.OCFastMode 			= TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(tim_handle, &sConfigOC, channel);
    HAL_TIM_PWM_Start(tim_handle, channel);
}

/* -------------------------------------------------------------------------- */

//TIM Base Functions
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
	if( tim_baseHandle->Instance == TIM10 )
	{
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM10);
        hal_gpio_init_alternate( _FAN_PWM, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

    }

	if( tim_baseHandle->Instance == TIM11 )
	{
		LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_TIM11);
        hal_gpio_init_alternate( _BUZZER, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
HAL_TIM_Base_MspDeInit( TIM_HandleTypeDef* tim_baseHandle )
{
    if(tim_baseHandle->Instance==TIM10)
    {
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM10);
    }
    else if(tim_baseHandle->Instance==TIM11)
    {
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM11);

    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
HAL_TIM_MspPostInit( TIM_HandleTypeDef* htim )
{
	if( htim->Instance == TIM10 )
	{
        hal_gpio_init_alternate( _FAN_PWM, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
	}

	if( htim->Instance == TIM11 )
	{
        hal_gpio_init_alternate( _BUZZER, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
	}

	if( htim->Instance == TIM2 )
	{
        hal_gpio_init_alternate( _AUX_PWM_0, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_1, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
	}

	if( htim->Instance==TIM12 )
	{
        hal_gpio_init_alternate( _AUX_PWM_1, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_9, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
        hal_gpio_init_alternate( _AUX_PWM_2, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_9, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
	}

}

// TIM Group 3 - PWM Input
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{
    if( tim_pwmHandle->Instance == TIM1 )
    {
        LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_TIM1);
        hal_gpio_init_alternate( _SERVO_3_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_1, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
    }

    if( tim_pwmHandle->Instance == TIM3 )
    {
        LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM3);
        hal_gpio_init_alternate( _SERVO_1_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
    }

    if( tim_pwmHandle->Instance == TIM4 )
    {
        LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM4);
        hal_gpio_init_alternate( _SERVO_2_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
    }

    if( tim_pwmHandle->Instance == TIM5 )
    {
        LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM5);
        hal_gpio_init_alternate( _SERVO_4_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
    }

    if( tim_pwmHandle->Instance == TIM9 )
    {
        LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_TIM9);
        hal_gpio_init_alternate( _FAN_TACHO, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
    }
}
/* ----- End ---------------------------------------------------------------- */
