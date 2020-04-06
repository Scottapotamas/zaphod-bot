/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <stm32f4xx_ll_rcc.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"

#include "hal_hard_ic.h"
#include "hal_gpio.h"
#include "hal_systick.h"
#include "qassert.h"


/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

#define HLFB_PRESCALE (84-1) // AHB speed is 84Mhz, therefore prescale for 1us per count
#define HLFB_PERIOD 0xFFFF

/* ----- Variables ---------------------------------------------------------- */

//Timers allocated to servos (HLFB on CH1/2 as PWM input, Output A and B are ch 3, 4
//TIM_HandleTypeDef htim1;    // HLFB 3 - TIM1_1/2
//TIM_HandleTypeDef htim3;    // HLFB 1 - TIM3_1/2
//TIM_HandleTypeDef htim4;    // HLFB 2 - TIM4_1/2
//TIM_HandleTypeDef htim5;    // HLFB 4 - TIM5_1/2

// Fan Hall Input
//TIM_HandleTypeDef htim9;    // TACH    - TIM9_1

typedef struct
{
    uint32_t cnt_a;
    uint32_t cnt_b;

    uint32_t tim_clock;
    uint32_t tim_prescale;
    uint32_t ic_prescale;

    uint8_t trigger_per_cycle;  // 1 for rising/falling edge, 2 for both edge trigger

    bool first_edge_done;

} HalHardICIntermediate_t;

PRIVATE HalHardICIntermediate_t ic_state[HAL_HARD_IC_NUM];  // holding values used to calculate edge durations
PRIVATE uint32_t                ic_values[HAL_HARD_IC_NUM]; // raw values per calculation

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
hal_setup_capture(uint8_t input);

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
hal_hard_ic_init( void )
{
    memset( &ic_values,    0, sizeof( ic_values ) );

    ic_state[HAL_HARD_IC_FAN_HALL].tim_clock = SystemCoreClock / 2;

    hal_setup_capture(HAL_HARD_IC_FAN_HALL );
//    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_1 );
//    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_2 );
//    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_3 );
//    hal_setup_capture( HAL_HARD_IC_HLFB_SERVO_4 );
}

/* -------------------------------------------------------------------------- */

/*
PUBLIC uint32_t
hal_hard_ic_read( InputCaptureSignal_t input )
{
    REQUIRE(input < HAL_HARD_IC_NUM );
    return ic_values[input];
}
*/

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_setup_capture(uint8_t input)
{

	switch (input)
	{
		case HAL_HARD_IC_HLFB_SERVO_1:
//			tim_handle = &htim3;
//			tim_handle->Instance = TIM3;
//
//            tim_handle->Init.Prescaler  = HLFB_PRESCALE;
//            tim_handle->Init.Period     = HLFB_PERIOD;

//            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
//            hal_gpio_init_alternate( _SERVO_1_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
//
//            HAL_NVIC_SetPriority( TIM3_IRQn, 4, 2 );
//            HAL_NVIC_EnableIRQ( TIM3_IRQn );

            break;

		case HAL_HARD_IC_HLFB_SERVO_2:
//			tim_handle = &htim4;
//			tim_handle->Instance = TIM4;
//
//            tim_handle->Init.Prescaler  = HLFB_PRESCALE;
//            tim_handle->Init.Period     = HLFB_PERIOD;

//            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
//            hal_gpio_init_alternate( _SERVO_2_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
//
//            HAL_NVIC_SetPriority( TIM4_IRQn, 4, 3 );
//            HAL_NVIC_EnableIRQ( TIM4_IRQn );

            break;

		case HAL_HARD_IC_HLFB_SERVO_3:
//			tim_handle = &htim1;
//			tim_handle->Instance = TIM1;
//
//            tim_handle->Init.Prescaler  = HLFB_PRESCALE;
//            tim_handle->Init.Period     = HLFB_PERIOD;

//            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
//            hal_gpio_init_alternate( _SERVO_3_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_1, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
//
//            HAL_NVIC_SetPriority( TIM1_CC_IRQn, 4, 1 );
//            HAL_NVIC_EnableIRQ( TIM1_CC_IRQn );

            break;

		case HAL_HARD_IC_HLFB_SERVO_4:
//			tim_handle = &htim5;
//			tim_handle->Instance = TIM5;
//
//            tim_handle->Init.Prescaler  = HLFB_PRESCALE;
//            tim_handle->Init.Period     = HLFB_PERIOD;

//            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
//            hal_gpio_init_alternate( _SERVO_4_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );
//
//            HAL_NVIC_SetPriority( TIM5_IRQn, 4, 4 );
//            HAL_NVIC_EnableIRQ( TIM5_IRQn );
            break;

		case HAL_HARD_IC_FAN_HALL:
			//TIM9;
            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM9);

            hal_gpio_init_alternate( _FAN_TACHO, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

            NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 6, 4));
            NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);

            // Timer Clock Configuration
            LL_TIM_SetClockDivision(TIM9, LL_TIM_CLOCKDIVISION_DIV4);
            LL_TIM_SetPrescaler(TIM9, 200);     //__LL_TIM_CALC_PSC(SystemCoreClock, 10000));
            // LL_TIM_SetAutoReload(TIM9, __LL_TIM_CALC_ARR(SystemCoreClock/2, LL_TIM_GetPrescaler(TIM9), 10));

            // Master/Slave Configuration
            LL_TIM_DisableMasterSlaveMode(TIM9);

            // Input Capture Configuration
            LL_TIM_IC_SetActiveInput(TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI);
            LL_TIM_IC_SetFilter(TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
            LL_TIM_IC_SetPrescaler(TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
            LL_TIM_IC_SetPolarity(TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);

            // Store the edge-trigger setting so we know how many triggers occur per period
            if (LL_TIM_IC_GetPolarity(TIM9, LL_TIM_CHANNEL_CH1) == LL_TIM_IC_POLARITY_BOTHEDGE)
            {
                ic_state[HAL_HARD_IC_FAN_HALL].trigger_per_cycle = 2;
            }
            else
            {
                ic_state[HAL_HARD_IC_FAN_HALL].trigger_per_cycle = 1;
            }

            // Store the prescaler values for faster access in interrupts
            ic_state[HAL_HARD_IC_FAN_HALL].tim_prescale = LL_TIM_GetPrescaler(TIM9);   // Timer prescale ratio
            ic_state[HAL_HARD_IC_FAN_HALL].ic_prescale = __LL_TIM_GET_ICPSC_RATIO(LL_TIM_IC_GetPrescaler(TIM9, LL_TIM_CHANNEL_CH1)); // Input capture prescale ratio

            // Enable Counter, channel and interrupt
            LL_TIM_EnableIT_CC1(TIM9);
            LL_TIM_CC_EnableChannel(TIM9, LL_TIM_CHANNEL_CH1);
            LL_TIM_EnableCounter(TIM9);

            break;

        default:
            ASSERT(false);
            break;
    }

}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_hard_ic_read( InputCaptureSignal_t input )
{
    return ic_values[input];
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
    return 0;//(hal_systick_get_ms() <= ic_state[signal].timestamp + timeout );
}

/* -------------------------------------------------------------------------- */

/*void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
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

}*/

/*PRIVATE void
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
}*/


void TIM1_BRK_TIM9_IRQHandler(void)
{
    // CC1 pending
    if(LL_TIM_IsActiveFlag_CC1(TIM9) == 1)
    {
        LL_TIM_ClearFlag_CC1(TIM9); // Clear the update interrupt flag

        static uint32_t cnt_delta = 0;

        if( !ic_state[HAL_HARD_IC_FAN_HALL].first_edge_done ) // First edge timestamp
        {
            ic_state[HAL_HARD_IC_FAN_HALL].cnt_a = LL_TIM_IC_GetCaptureCH1(TIM9);
            ic_state[HAL_HARD_IC_FAN_HALL].first_edge_done = true;
        }
        else if( ic_state[HAL_HARD_IC_FAN_HALL].first_edge_done ) // Second edge
        {
            ic_state[HAL_HARD_IC_FAN_HALL].cnt_b = LL_TIM_IC_GetCaptureCH1(TIM9);

            // Calculate the counts elapsed
            if (ic_state[HAL_HARD_IC_FAN_HALL].cnt_b > ic_state[HAL_HARD_IC_FAN_HALL].cnt_a)
            {
                cnt_delta = (ic_state[HAL_HARD_IC_FAN_HALL].cnt_b - ic_state[HAL_HARD_IC_FAN_HALL].cnt_a);
            }
            else if (ic_state[HAL_HARD_IC_FAN_HALL].cnt_b < ic_state[HAL_HARD_IC_FAN_HALL].cnt_a)
            {
                cnt_delta = ((0xFFFFU - ic_state[HAL_HARD_IC_FAN_HALL].cnt_a) + ic_state[HAL_HARD_IC_FAN_HALL].cnt_b) + 1;
            }
            else    // overflowing capture - need to adjust timer prescale value
            {
                cnt_delta = 0;
                ASSERT(false);
            }

            // Calculate the signal frequency
            ic_values[HAL_HARD_IC_FAN_HALL] = (ic_state[HAL_HARD_IC_FAN_HALL].tim_clock * ic_state[HAL_HARD_IC_FAN_HALL].ic_prescale )
                                              / ( cnt_delta * (ic_state[HAL_HARD_IC_FAN_HALL].tim_prescale+1) * ic_state[HAL_HARD_IC_FAN_HALL].trigger_per_cycle );

            ic_state[HAL_HARD_IC_FAN_HALL].first_edge_done = false;
        }
    }
}

/* ----- End ---------------------------------------------------------------- */
