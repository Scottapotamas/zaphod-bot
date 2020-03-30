/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

#include "hal_hard_ic.h"
#include "hal_gpio.h"
#include "hal_systick.h"
#include "qassert.h"

#include "stm32f4xx_hal.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

#define HLFB_PRESCALE (84-1) // AHB speed is 84Mhz, therefore prescale for 1us per count
#define HLFB_PERIOD 0xFFFF

/* ----- Variables ---------------------------------------------------------- */

//Timers allocated to servos (HLFB on CH1/2 as PWM input, Output A and B are ch 3, 4
TIM_HandleTypeDef htim1;    // HLFB 3 - TIM1_1/2
TIM_HandleTypeDef htim3;    // HLFB 1 - TIM3_1/2
TIM_HandleTypeDef htim4;    // HLFB 2 - TIM4_1/2
TIM_HandleTypeDef htim5;    // HLFB 4 - TIM5_1/2

// Fan Hall Input
TIM_HandleTypeDef htim9;    // TACH    - TIM9_1

typedef struct
{
    uint32_t cnt_a;
    uint32_t cnt_b;
    uint32_t timestamp;
    bool first_edge_done;
} HalHardICIntermediate_t;

PRIVATE HalHardICIntermediate_t ic_state[HAL_HARD_IC_NUM];  // holding values used to calculate edge durations
PRIVATE uint32_t                ic_values[HAL_HARD_IC_NUM]; // raw values per calculation

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
hal_setup_capture(uint8_t input);

PRIVATE void
hal_hard_ic_process_duty_cycle( TIM_HandleTypeDef *htim, InputCaptureSignal_t signal );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
hal_hard_ic_init( void )
{
    memset( &ic_values,    0, sizeof( ic_values ) );

    hal_setup_capture(HAL_HARD_IC_FAN_HALL );
//    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_1 );
//    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_2 );
//    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_3 );
//    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_4 );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_hard_ic_read( InputCaptureSignal_t input )
{
    REQUIRE(input < HAL_HARD_IC_NUM );
    return ic_values[input];
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_setup_capture(uint8_t input)
{
	TIM_HandleTypeDef* tim_handle = 0;
    TIM_IC_InitTypeDef sConfigIC;
    TIM_MasterConfigTypeDef sMasterConfig;
//    TIM_SlaveConfigTypeDef sSlaveConfig;

	switch (input)
	{
		case HAL_HARD_IC_HLFB_SERVO_1:
			tim_handle = &htim3;
			tim_handle->Instance = TIM3;

            tim_handle->Init.Prescaler  = HLFB_PRESCALE;
            tim_handle->Init.Period     = HLFB_PERIOD;
            break;

		case HAL_HARD_IC_HLFB_SERVO_2:
			tim_handle = &htim4;
			tim_handle->Instance = TIM4;

            tim_handle->Init.Prescaler  = HLFB_PRESCALE;
            tim_handle->Init.Period     = HLFB_PERIOD;
            break;

		case HAL_HARD_IC_HLFB_SERVO_3:
			tim_handle = &htim1;
			tim_handle->Instance = TIM1;

            tim_handle->Init.Prescaler  = HLFB_PRESCALE;
            tim_handle->Init.Period     = HLFB_PERIOD;
            break;

		case HAL_HARD_IC_HLFB_SERVO_4:
			tim_handle = &htim5;
			tim_handle->Instance = TIM5;

            tim_handle->Init.Prescaler  = HLFB_PRESCALE;
            tim_handle->Init.Period     = HLFB_PERIOD;
            break;

		case HAL_HARD_IC_FAN_HALL:
			tim_handle                  = &htim9;
			tim_handle->Instance        = TIM9;

            tim_handle->Init.Period     = 0xFFFF;
            tim_handle->Init.Prescaler  = 200;
            break;
    }

    // Setup the Timer
    tim_handle->Init.ClockDivision 		= TIM_CLOCKDIVISION_DIV1;
    tim_handle->Init.CounterMode 		= TIM_COUNTERMODE_UP;
    tim_handle->Init.RepetitionCounter 	= 0;

    if (HAL_TIM_IC_Init(tim_handle) != HAL_OK)
    {
       ASSERT( false );
    }

    // Setup the Master as needed
    if( input != HAL_HARD_IC_FAN_HALL)
    {
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(tim_handle, &sMasterConfig) != HAL_OK)
        {
           ASSERT( false );
        }
    }

    // Setup the Input Capture Channel
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    if (HAL_TIM_IC_ConfigChannel(tim_handle, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
    {
       ASSERT( false );
    }

    // Start the Input capture in Interrupt mode
    HAL_TIM_IC_Start_IT(tim_handle, TIM_CHANNEL_1);
}

/* -------------------------------------------------------------------------- */

// TIM Group 4 - Input Capture
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* tim_icHandle)
{

	if( tim_icHandle->Instance == TIM1 )
	{
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
        hal_gpio_init_alternate( _SERVO_3_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_1, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

        HAL_NVIC_SetPriority( TIM1_CC_IRQn, 4, 1 );
		HAL_NVIC_EnableIRQ( TIM1_CC_IRQn );
		//todo work out if the interrupt assignment here is correct
	}

	if( tim_icHandle->Instance == TIM3 )
	{
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
        hal_gpio_init_alternate( _SERVO_1_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

        HAL_NVIC_SetPriority( TIM3_IRQn, 4, 2 );
		HAL_NVIC_EnableIRQ( TIM3_IRQn );
	}

	if( tim_icHandle->Instance == TIM4 )
	{
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
        hal_gpio_init_alternate( _SERVO_2_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

		HAL_NVIC_SetPriority( TIM4_IRQn, 4, 3 );
		HAL_NVIC_EnableIRQ( TIM4_IRQn );
	}

	if( tim_icHandle->Instance == TIM5 )
	{
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
        hal_gpio_init_alternate( _SERVO_4_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

		HAL_NVIC_SetPriority( TIM5_IRQn, 4, 4 );
		HAL_NVIC_EnableIRQ( TIM5_IRQn );
	}

	if( tim_icHandle->Instance == TIM9 )
    {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM9);
        hal_gpio_init_alternate( _FAN_TACHO, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

		HAL_NVIC_SetPriority( TIM1_BRK_TIM9_IRQn, 6, 3 );
		HAL_NVIC_EnableIRQ( TIM1_BRK_TIM9_IRQn );
    }

}

/* -------------------------------------------------------------------------- */

// TIM Group 3 - PWM Input
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{
	if(tim_pwmHandle->Instance==TIM1)
	{
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM1);
		hal_gpio_disable_pin( _SERVO_3_HLFB );
	}
	else if(tim_pwmHandle->Instance==TIM2)
	{
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM2);
	}
	else if(tim_pwmHandle->Instance==TIM3)
	{
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM3);
		hal_gpio_disable_pin( _SERVO_1_HLFB );
	}
	else if(tim_pwmHandle->Instance==TIM4)
	{
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM4);
		hal_gpio_disable_pin( _SERVO_2_HLFB );
	}
	else if(tim_pwmHandle->Instance==TIM5)
	{
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM5);
		hal_gpio_disable_pin( _SERVO_4_HLFB );
	}
	else if(tim_pwmHandle->Instance==TIM12)
	{
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM12);
	}
}

// TIM Group 4 - Input Capture
void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* tim_icHandle)
{
	if(tim_icHandle->Instance==TIM9)
	{
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM9);

		hal_gpio_disable_pin( _FAN_TACHO );
        HAL_NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);
    }
}

/* -------------------------------------------------------------------------- */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{


}

/* -------------------------------------------------------------------------- */

#define IC_TIMEOUT_HLFB 21 // milliseconds (50Hz signal)
#define IC_TIMEOUT_FAN 100 // milliseconds

// Performs a check if the IC callbacks has been firing recently to determine
// if the IC value is valid
PUBLIC bool hal_hard_ic_is_recent( InputCaptureSignal_t signal )
{
    // Choose a timeout appropriate for the input signal frequency
    uint16_t timeout = ( signal == HAL_HARD_IC_FAN_HALL )? IC_TIMEOUT_FAN : IC_TIMEOUT_HLFB;

    // returns false if the IC hasn't had a new event within the timeout period
    return (hal_systick_get_ms() <= ic_state[signal].timestamp + timeout );
}

/* -------------------------------------------------------------------------- */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    // Store timestamps of rising/falling edges,
    // calculate duty cycles for HLFB signals
    // Calculate frequency for fan tachometer

	if(htim->Instance==TIM1)
	{
        hal_hard_ic_process_duty_cycle(htim, HAL_HARD_IC_HLFB_SERVO_3);
	}
	else if(htim->Instance==TIM3)
	{
        hal_hard_ic_process_duty_cycle(htim, HAL_HARD_IC_HLFB_SERVO_1);
	}
	else if(htim->Instance==TIM4)
	{
        hal_hard_ic_process_duty_cycle(htim, HAL_HARD_IC_HLFB_SERVO_2);
	}
	else if(htim->Instance==TIM5)
	{
        hal_hard_ic_process_duty_cycle(htim, HAL_HARD_IC_HLFB_SERVO_4);
    }
	else if(htim->Instance==TIM9)
	{
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            // Measure frequency of the hall effect sensor

            // Catch the first edge to start our timing reference
            if( !ic_state[HAL_HARD_IC_FAN_HALL].first_edge_done )
            {
                ic_state[HAL_HARD_IC_FAN_HALL].cnt_a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
                ic_state[HAL_HARD_IC_FAN_HALL].first_edge_done = true;
            }
            else    // second edge is the other half of the square-wave
            {
                uint32_t delta_counts = 0;
                ic_state[HAL_HARD_IC_FAN_HALL].cnt_b = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

                // Calculate the count delta between the two edges
                if (ic_state[HAL_HARD_IC_FAN_HALL].cnt_b > ic_state[HAL_HARD_IC_FAN_HALL].cnt_a) {
                    delta_counts = ic_state[HAL_HARD_IC_FAN_HALL].cnt_b - ic_state[HAL_HARD_IC_FAN_HALL].cnt_a;
                }
                else if (ic_state[HAL_HARD_IC_FAN_HALL].cnt_b < ic_state[HAL_HARD_IC_FAN_HALL].cnt_a)
                {
                    delta_counts = ((0xFFFF - ic_state[HAL_HARD_IC_FAN_HALL].cnt_a) + ic_state[HAL_HARD_IC_FAN_HALL].cnt_b) + 1;
                }

                ic_values[HAL_HARD_IC_FAN_HALL] = (HAL_RCC_GetPCLK2Freq() / delta_counts) / 100;

                ic_state[HAL_HARD_IC_FAN_HALL].first_edge_done = false;   // reset to catch the next 'new' edge.
            }

            ic_state[HAL_HARD_IC_FAN_HALL].timestamp = hal_systick_get_ms();
        }
	}

}

PRIVATE void
hal_hard_ic_process_duty_cycle( TIM_HandleTypeDef *htim, InputCaptureSignal_t signal )
{
    // All HLFB signals are on channel 1 of their respective timers
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        if (!ic_state[signal].first_edge_done) // if the first value is not captured
        {
            ic_state[signal].cnt_a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
            ic_state[signal].first_edge_done = true;

            // Change the polarity to falling edge
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else
        {
            ic_state[signal].cnt_b = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
            __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

            if (ic_state[signal].cnt_b > ic_state[signal].cnt_a)
            {
                ic_values[signal] = ic_state[signal].cnt_b - ic_state[signal].cnt_a;;
            }

            ic_state[signal].first_edge_done = false;

            // set polarity to rising edge
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
        }
        ic_state[signal].timestamp = hal_systick_get_ms();
    }
}

/* ----- End ---------------------------------------------------------------- */
