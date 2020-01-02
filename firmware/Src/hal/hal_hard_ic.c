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
    bool first_edge_done;
} HalHardICIntermediate_t;

PRIVATE HalHardICIntermediate_t ic_state[HAL_HARD_IC_NUM];  // holding values used to calculate edge durations
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

    average_short_init(&ic_averages[HAL_HARD_IC_FAN_HALL], 5 );
    average_short_init( &ic_averages[HAL_HARD_IC_HLFB_SERVO_1],    30 );
    average_short_init( &ic_averages[HAL_HARD_IC_HLFB_SERVO_2],    30 );
    average_short_init( &ic_averages[HAL_HARD_IC_HLFB_SERVO_3],    30 );
    average_short_init( &ic_averages[HAL_HARD_IC_HLFB_SERVO_4],    30 );

    hal_setup_capture(HAL_HARD_IC_FAN_HALL );
    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_1 );
    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_2 );
    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_3 );

    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_4 );
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
        _Error_Handler(__FILE__, __LINE__);
    }

    // Setup the Master as needed
    if( input != HAL_HARD_IC_FAN_HALL)
    {
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(tim_handle, &sMasterConfig) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }
    }

    // Setup the Input Capture Channel
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

    // Make sure the IO is setup properly
//    HAL_TIM_MspPostInit(tim_handle);
}

/* -------------------------------------------------------------------------- */

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

// TIM Group 3 - PWM Input
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{
	if(tim_pwmHandle->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_DISABLE();

		hal_gpio_disable_pin( _SERVO_3_HLFB );
//		hal_gpio_disable_pin( _SERVO_3_A );
//		hal_gpio_disable_pin( _SERVO_3_B );
	}
	else if(tim_pwmHandle->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_DISABLE();
	}
	else if(tim_pwmHandle->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_DISABLE();

		hal_gpio_disable_pin( _SERVO_1_HLFB );
//		hal_gpio_disable_pin( _SERVO_1_A );
//		hal_gpio_disable_pin( _SERVO_1_B );
	}
	else if(tim_pwmHandle->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_DISABLE();

		hal_gpio_disable_pin( _SERVO_2_HLFB );
//		hal_gpio_disable_pin( _SERVO_2_A );
//		hal_gpio_disable_pin( _SERVO_2_B );
	}
	else if(tim_pwmHandle->Instance==TIM5)
	{
		__HAL_RCC_TIM5_CLK_DISABLE();

		hal_gpio_disable_pin( _SERVO_4_HLFB );
//		hal_gpio_disable_pin( _SERVO_4_A );
//		hal_gpio_disable_pin( _SERVO_4_B );
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

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//
//}

/* -------------------------------------------------------------------------- */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
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
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if (!ic_state[HAL_HARD_IC_HLFB_SERVO_4].first_edge_done) // if the first value is not captured
            {
                ic_state[HAL_HARD_IC_HLFB_SERVO_4].cnt_a = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
                ic_state[HAL_HARD_IC_HLFB_SERVO_4].first_edge_done = true;

                // Change the polarity to falling edge
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
            }
            else
            {
                ic_state[HAL_HARD_IC_HLFB_SERVO_4].cnt_b = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
                __HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

                if (ic_state[HAL_HARD_IC_HLFB_SERVO_4].cnt_b > ic_state[HAL_HARD_IC_HLFB_SERVO_4].cnt_a)
                {
                    ic_values[HAL_HARD_IC_HLFB_SERVO_4] = ic_state[HAL_HARD_IC_HLFB_SERVO_4].cnt_b - ic_state[HAL_HARD_IC_HLFB_SERVO_4].cnt_a;;
                    average_short_update(&ic_averages[HAL_HARD_IC_HLFB_SERVO_4], (uint16_t)ic_values[HAL_HARD_IC_HLFB_SERVO_4] );
                    ic_peaks[HAL_HARD_IC_HLFB_SERVO_4] = MAX(ic_peaks[HAL_HARD_IC_HLFB_SERVO_4], ic_values[HAL_HARD_IC_HLFB_SERVO_4] );
                }

                ic_state[HAL_HARD_IC_HLFB_SERVO_4].first_edge_done = false;

                // set polarity to rising edge
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
            }
        }
	}
	else if(htim->Instance==TIM9)
	{
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {

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
                average_short_update(&ic_averages[HAL_HARD_IC_FAN_HALL], (uint16_t)ic_values[HAL_HARD_IC_FAN_HALL] );
                ic_peaks[HAL_HARD_IC_FAN_HALL] = MAX(ic_peaks[HAL_HARD_IC_FAN_HALL], ic_values[HAL_HARD_IC_FAN_HALL] );

                ic_state[HAL_HARD_IC_FAN_HALL].first_edge_done = false;   // reset to catch the next 'new' edge.
            }
        }
	}

}

/* ----- End ---------------------------------------------------------------- */
