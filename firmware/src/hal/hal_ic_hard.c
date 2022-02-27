/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_tim.h"

#include "hal_gpio.h"
#include "hal_ic_hard.h"
#include "qassert.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

#define FAN_IC_PRESCALE         1U
#define FAN_TIM_PRESCALE        250U
#define FAN_TIM_CLOCK           84000000UL
#define FAN_IC_EDGES_PER_PERIOD 1

/* ----- Variables ---------------------------------------------------------- */

typedef struct
{
    uint32_t cnt_a;
    uint32_t cnt_b;

    bool first_edge_done;

} HalHardICIntermediate_t;

PRIVATE HalHardICIntermediate_t fan_state;                     // holding values used to calculate edge durations
PRIVATE uint32_t                ic_values[HAL_IC_HARD_NUM];    // Calculated duty cycle or frequency values, x100 for precision

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
hal_ic_hard_configure_pwm_input( TIM_TypeDef *TIMx );

PRIVATE void
hal_ic_hard_pwmic_irq_handler( InputCaptureSignal_t input, TIM_TypeDef *TIMx );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
hal_ic_hard_init( void )
{
    memset( &fan_state, 0, sizeof( fan_state ) );
    memset( &ic_values, 0, sizeof( ic_values ) );

    hal_setup_capture( HAL_IC_HARD_FAN_HALL );
    hal_setup_capture( HAL_IC_HARD_HLFB_SERVO_1 );
    hal_setup_capture( HAL_IC_HARD_HLFB_SERVO_2 );
    hal_setup_capture( HAL_IC_HARD_HLFB_SERVO_3 );
    hal_setup_capture( HAL_IC_HARD_HLFB_SERVO_4 );
}

/* -------------------------------------------------------------------------- */

// Uncomment this define to use the TIM3 peripheral for HLFB1, otherwise TIM8 is used.
// TIM8 might be needed for more important uses elsewhere, as TIM8 is one of the few "enhanced functionality" timers
//#define HLFB_TIM_3 1

PUBLIC void
hal_setup_capture( uint8_t input )
{
    switch( input )
    {
        case HAL_IC_HARD_HLFB_SERVO_1:
            // TIM3 or TIM8

#ifdef HLFB_TIM_3
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM3 );

            hal_gpio_init_alternate( _SERVO_1_HLFB, LL_GPIO_MODE_ALTERNATE, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

            NVIC_SetPriority( TIM3_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 8, 2 ) );
            NVIC_EnableIRQ( TIM3_IRQn );

            hal_ic_hard_configure_pwm_input( TIM3 );
#else
            LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_TIM8 );

            hal_gpio_init_alternate( _SERVO_1_HLFB, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

            NVIC_SetPriority( TIM8_CC_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 8, 2 ) );
            NVIC_EnableIRQ( TIM8_CC_IRQn );

            hal_ic_hard_configure_pwm_input( TIM8 );
#endif
            break;

        case HAL_IC_HARD_HLFB_SERVO_2:
            // TIM4
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM4 );

            hal_gpio_init_alternate( _SERVO_2_HLFB, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

            NVIC_SetPriority( TIM4_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 8, 2 ) );
            NVIC_EnableIRQ( TIM4_IRQn );

            hal_ic_hard_configure_pwm_input( TIM4 );
            break;

        case HAL_IC_HARD_HLFB_SERVO_3:
            // TIM1
            LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_TIM1 );

            hal_gpio_init_alternate( _SERVO_3_HLFB, LL_GPIO_AF_1, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

            NVIC_SetPriority( TIM1_CC_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 8, 2 ) );
            NVIC_EnableIRQ( TIM1_CC_IRQn );

            hal_ic_hard_configure_pwm_input( TIM1 );
            break;

        case HAL_IC_HARD_HLFB_SERVO_4:
            // TIM5
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM5 );

            hal_gpio_init_alternate( _SERVO_4_HLFB, LL_GPIO_AF_2, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

            NVIC_SetPriority( TIM5_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 8, 2 ) );
            NVIC_EnableIRQ( TIM5_IRQn );

            hal_ic_hard_configure_pwm_input( TIM5 );
            break;

        case HAL_IC_HARD_FAN_HALL:
            //TIM9;
            LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_TIM9 );

            hal_gpio_init_alternate( _FAN_TACHO, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_LOW, LL_GPIO_PULL_NO );

            NVIC_SetPriority( TIM1_BRK_TIM9_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 8, 1 ) );
            NVIC_EnableIRQ( TIM1_BRK_TIM9_IRQn );

            // Timer Clock Configuration
            LL_TIM_SetClockDivision( TIM9, LL_TIM_CLOCKDIVISION_DIV4 );
            LL_TIM_SetPrescaler( TIM9, FAN_TIM_PRESCALE );    //__LL_TIM_CALC_PSC(SystemCoreClock, 10000));
            // LL_TIM_SetAutoReload(TIM9, __LL_TIM_CALC_ARR(SystemCoreClock/2, LL_TIM_GetPrescaler(TIM9), 10));

            LL_TIM_DisableMasterSlaveMode( TIM9 );

            // Input Capture Configuration
            LL_TIM_IC_SetActiveInput( TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI );
            LL_TIM_IC_SetFilter( TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV16_N8 );
            LL_TIM_IC_SetPrescaler( TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1 );    // FAN_IC_PRESCALE
            LL_TIM_IC_SetPolarity( TIM9, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING );

            // Store the edge-trigger setting to know how many triggers occur per period
            if( LL_TIM_IC_GetPolarity( TIM9, LL_TIM_CHANNEL_CH1 ) == LL_TIM_IC_POLARITY_BOTHEDGE )
            {
                // rising AND falling edge
                ASSERT( FAN_IC_EDGES_PER_PERIOD == 2 );
            }
            else
            {
                // Just the rising OR falling edge
                ASSERT( FAN_IC_EDGES_PER_PERIOD == 1 );
            }

            // Check that we've defined the prescale divisor correctly
            ASSERT( FAN_IC_PRESCALE == __LL_TIM_GET_ICPSC_RATIO( LL_TIM_IC_GetPrescaler( TIM9, LL_TIM_CHANNEL_CH1 ) ) );

            // Check that we've defined the timer prescaler value correctly
            ASSERT( FAN_TIM_PRESCALE == LL_TIM_GetPrescaler( TIM9 ) );

            // Enable Counter, channel and interrupt
            LL_TIM_EnableIT_CC1( TIM9 );
            LL_TIM_CC_EnableChannel( TIM9, LL_TIM_CHANNEL_CH1 );
            LL_TIM_EnableCounter( TIM9 );

            break;

        default:
            ASSERT( false );
            break;
    }
}

PRIVATE void
hal_ic_hard_configure_pwm_input( TIM_TypeDef *TIMx )
{
    // Timer Clock Configuration
    LL_TIM_SetClockDivision( TIMx, LL_TIM_CLOCKDIVISION_DIV4 );

    // The prescaler is configured such that the 45Hz squarewave from the Clearpath servo's _just_ fits within the 16-bit counter
    LL_TIM_SetPrescaler( TIMx, 60 );
    LL_TIM_SetCounterMode( TIMx, LL_TIM_COUNTERMODE_UP );
    //LL_TIM_SetAutoReload(TIMx, 0xFFFF);

    // IC Channel 1
    LL_TIM_IC_SetFilter( TIMx, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV32_N8 );
    LL_TIM_IC_SetPrescaler( TIMx, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1 );
    LL_TIM_IC_SetPolarity( TIMx, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING );
    LL_TIM_IC_SetActiveInput( TIMx, LL_TIM_CHANNEL_CH1, LL_TIM_ACTIVEINPUT_DIRECTTI );

    // IC Channel 2
    LL_TIM_IC_SetFilter( TIMx, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV32_N8 );
    LL_TIM_IC_SetPrescaler( TIMx, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1 );
    LL_TIM_IC_SetPolarity( TIMx, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_FALLING );
    LL_TIM_IC_SetActiveInput( TIMx, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_INDIRECTTI );

    // Configure Slave Reset if supported
    if( IS_TIM_SLAVE_INSTANCE( TIMx ) )
    {
        LL_TIM_SetSlaveMode( TIMx, LL_TIM_SLAVEMODE_RESET );
        LL_TIM_SetTriggerInput( TIMx, LL_TIM_TS_TI1FP1 );
        LL_TIM_ConfigETR( TIMx, LL_TIM_ETR_POLARITY_NONINVERTED, LL_TIM_ETR_PRESCALER_DIV1, LL_TIM_ETR_FILTER_FDIV32_N8 );
        //        LL_TIM_EnableMasterSlaveMode(TIMx);
        LL_TIM_DisableMasterSlaveMode( TIMx );
    }
    else
    {
        ASSERT( false );    // wut, maybe look into doing the counter reset/sw-diffing as part of CC2 IRQ?
    }

    // Enable interrupts, channels, and counter
    LL_TIM_EnableIT_CC2( TIMx );
    LL_TIM_EnableIT_CC1( TIMx );

    LL_TIM_CC_EnableChannel( TIMx, LL_TIM_CHANNEL_CH1 );
    LL_TIM_CC_EnableChannel( TIMx, LL_TIM_CHANNEL_CH2 );

    LL_TIM_EnableCounter( TIMx );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_ic_hard_read( InputCaptureSignal_t input )
{
    return ic_values[input] / 100;
}

PUBLIC float
hal_ic_hard_read_f( InputCaptureSignal_t input )
{
    return (float)ic_values[input] / 100;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_ic_hard_pwmic_irq_handler( InputCaptureSignal_t input, TIM_TypeDef *TIMx )
{
    if( LL_TIM_IsActiveFlag_CC1( TIMx ) )
    {
        LL_TIM_ClearFlag_CC1( TIMx );

        static uint32_t cnt_a = 0;
        cnt_a                 = LL_TIM_IC_GetCaptureCH1( TIMx );

        if( cnt_a != 0 )
        {
            uint32_t cnt_b   = LL_TIM_IC_GetCaptureCH2( TIMx );
            ic_values[input] = ( cnt_b * 10000 ) / ( cnt_a );

            // TODO optionally calculate frequency for this signal
            // uint32_t frequency = TimerClock  / (1*cnt_a);
        }
        else
        {
            ic_values[input] = 0;
        }
    }

    if( LL_TIM_IsActiveFlag_CC2( TIMx ) )
    {
        LL_TIM_ClearFlag_CC2( TIMx );
    }
}

// Servo 1 HLFB
void TIM8_CC_IRQHandler( void )
{
    hal_ic_hard_pwmic_irq_handler( HAL_IC_HARD_HLFB_SERVO_1, TIM8 );
}

void TIM3_IRQHandler( void )
{
    hal_ic_hard_pwmic_irq_handler( HAL_IC_HARD_HLFB_SERVO_1, TIM3 );
}

// Servo 2 HLFB
void TIM4_IRQHandler( void )
{
    hal_ic_hard_pwmic_irq_handler( HAL_IC_HARD_HLFB_SERVO_2, TIM4 );
}

// Servo 3 HLFB
void TIM1_CC_IRQHandler( void )
{
    hal_ic_hard_pwmic_irq_handler( HAL_IC_HARD_HLFB_SERVO_3, TIM1 );
}

// Servo 4 HLFB
void TIM5_IRQHandler( void )
{
    hal_ic_hard_pwmic_irq_handler( HAL_IC_HARD_HLFB_SERVO_4, TIM5 );
}

// Fan Hall sensor
void TIM1_BRK_TIM9_IRQHandler( void )
{
    if( LL_TIM_IsActiveFlag_CC1( TIM9 ) )
    {
        LL_TIM_ClearFlag_CC1( TIM9 );

        static uint32_t cnt_delta;

        if( !fan_state.first_edge_done )    // First edge timestamp
        {
            fan_state.cnt_a           = LL_TIM_IC_GetCaptureCH1( TIM9 );
            fan_state.first_edge_done = true;
        }
        else if( fan_state.first_edge_done )    // Second edge
        {
            fan_state.cnt_b = LL_TIM_IC_GetCaptureCH1( TIM9 );

            // Calculate the counts elapsed
            if( fan_state.cnt_b > fan_state.cnt_a )
            {
                cnt_delta = ( fan_state.cnt_b - fan_state.cnt_a );
            }
            else if( fan_state.cnt_b < fan_state.cnt_a )
            {
                cnt_delta = ( ( 0xFFFFU - fan_state.cnt_a ) + fan_state.cnt_b ) + 1;
            }
            else    // overflowing capture - need to adjust timer prescale value
            {
                ASSERT( false );
            }

            // Calculate the signal frequency
            ic_values[HAL_IC_HARD_FAN_HALL] = ( FAN_TIM_CLOCK * FAN_IC_PRESCALE * 100 )
                                              / ( cnt_delta * ( FAN_TIM_PRESCALE + 1 ) * FAN_IC_EDGES_PER_PERIOD );

            fan_state.first_edge_done = false;
        }
    }
}

/* ----- End ---------------------------------------------------------------- */
