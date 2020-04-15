/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"

#include "hal_gpio.h"
#include "hal_pwm.h"
#include "qassert.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

#define PWM_PERIOD_DEFAULT 1024

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

PRIVATE void
hal_pwm_configure_peripheral( TIM_TypeDef *TIMx, uint32_t channel, uint32_t frequency );

/* ----- Public Functions --------------------------------------------------- */

void hal_pwm_generation( PWMOutputTimerDef_t pwm_output, uint16_t frequency )
{
    switch( pwm_output )
    {
        case _PWM_TIM_FAN:
            LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_TIM10 );
            hal_gpio_init_alternate( _FAN_PWM, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_PULL_DOWN );

            NVIC_SetPriority( TIM1_UP_TIM10_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 7, 5 ) );
            NVIC_EnableIRQ( TIM1_UP_TIM10_IRQn );

            hal_pwm_configure_peripheral( TIM10, LL_TIM_CHANNEL_CH1, frequency );
            break;

        case _PWM_TIM_BUZZER:
            LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_TIM11 );
            hal_gpio_init_alternate( _BUZZER, LL_GPIO_AF_3, LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_PULL_DOWN );

            NVIC_SetPriority( TIM1_TRG_COM_TIM11_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 7, 4 ) );
            NVIC_EnableIRQ( TIM1_TRG_COM_TIM11_IRQn );

            hal_pwm_configure_peripheral( TIM11, LL_TIM_CHANNEL_CH1, frequency );
            break;

        case _PWM_TIM_AUX_0:
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM2 );
            hal_gpio_init_alternate( _AUX_PWM_0, LL_GPIO_AF_1, LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_PULL_DOWN );

            NVIC_SetPriority( TIM2_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 7, 3 ) );
            NVIC_EnableIRQ( TIM2_IRQn );

            hal_pwm_configure_peripheral( TIM2, LL_TIM_CHANNEL_CH1, frequency );
            break;

        case _PWM_TIM_AUX_1:
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM12 );
            hal_gpio_init_alternate( _AUX_PWM_1, LL_GPIO_AF_9, LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_PULL_DOWN );

            NVIC_SetPriority( TIM8_BRK_TIM12_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 7, 3 ) );
            NVIC_EnableIRQ( TIM8_BRK_TIM12_IRQn );

            hal_pwm_configure_peripheral( TIM12, LL_TIM_CHANNEL_CH2, frequency );
            break;

        case _PWM_TIM_AUX_2:
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_TIM12 );
            hal_gpio_init_alternate( _AUX_PWM_1, LL_GPIO_AF_9, LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_PULL_DOWN );

            NVIC_SetPriority( TIM8_BRK_TIM12_IRQn, NVIC_EncodePriority( NVIC_GetPriorityGrouping(), 7, 3 ) );
            NVIC_EnableIRQ( TIM8_BRK_TIM12_IRQn );

            hal_pwm_configure_peripheral( TIM12, LL_TIM_CHANNEL_CH1, frequency );
            break;
        default:
            ASSERT( false );
            break;
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_pwm_configure_peripheral( TIM_TypeDef *TIMx, uint32_t channel, uint32_t frequency )
{
    LL_TIM_SetPrescaler( TIMx, ( uint32_t )( SystemCoreClock / ( frequency * PWM_PERIOD_DEFAULT ) ) );
    LL_TIM_SetCounterMode( TIMx, LL_TIM_COUNTERMODE_UP );
    LL_TIM_SetAutoReload( TIMx, PWM_PERIOD_DEFAULT );
    LL_TIM_SetClockDivision( TIMx, LL_TIM_CLOCKDIVISION_DIV1 );
    LL_TIM_SetRepetitionCounter( TIMx, (uint8_t)0x00 );
    LL_TIM_EnableARRPreload( TIMx );

    LL_TIM_OC_SetMode( TIMx, channel, LL_TIM_OCMODE_PWM1 );
    LL_TIM_OC_SetPolarity( TIMx, channel, LL_TIM_OCPOLARITY_LOW );
    LL_TIM_OC_SetIdleState( TIMx, channel, LL_TIM_OCIDLESTATE_LOW );

    switch( channel )
    {
        case LL_TIM_CHANNEL_CH1:
            LL_TIM_OC_SetCompareCH1( TIMx, ( ( LL_TIM_GetAutoReload( TIMx ) + 1 ) / 2 ) );
            break;
        case LL_TIM_CHANNEL_CH2:
            LL_TIM_OC_SetCompareCH2( TIMx, ( ( LL_TIM_GetAutoReload( TIMx ) + 1 ) / 2 ) );
            break;
        case LL_TIM_CHANNEL_CH3:
            LL_TIM_OC_SetCompareCH2( TIMx, ( ( LL_TIM_GetAutoReload( TIMx ) + 1 ) / 2 ) );
            break;
        case LL_TIM_CHANNEL_CH4:
            LL_TIM_OC_SetCompareCH2( TIMx, ( ( LL_TIM_GetAutoReload( TIMx ) + 1 ) / 2 ) );
            break;
    }

    LL_TIM_OC_EnablePreload( TIMx, channel );

    // Enable output channel
    LL_TIM_CC_EnableChannel( TIMx, channel );
    LL_TIM_EnableCounter( TIMx );
    LL_TIM_GenerateEvent_UPDATE( TIMx );
}

/* -------------------------------------------------------------------------- */

// O-100% as a float
PUBLIC void hal_pwm_set_percentage_f( PWMOutputTimerDef_t pwm_output, float percentage )
{
    if( percentage >= 0.0f && percentage <= 100.0f )
    {
        uint16_t duty_as_u16 = ( percentage / 100.0f ) * 0xFFFFU;
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
    uint16_t duty_as_u16 = duty_cycle * 0xFFU;
    hal_pwm_set( pwm_output, duty_as_u16 );
}

/* -------------------------------------------------------------------------- */

PRIVATE void hal_pwm_set( PWMOutputTimerDef_t pwm_output, uint16_t duty_cycle )
{
    uint32_t signal_period = 0;

    switch( pwm_output )
    {
        case _PWM_TIM_FAN:
            signal_period = LL_TIM_GetAutoReload( TIM10 ) + 1;
            LL_TIM_OC_SetCompareCH1( TIM10, ( MIN( duty_cycle, 0xFFFFU ) * signal_period ) / 0xFFFFU );
            break;

        case _PWM_TIM_BUZZER:
            signal_period = LL_TIM_GetAutoReload( TIM11 ) + 1;
            LL_TIM_OC_SetCompareCH1( TIM11, ( MIN( duty_cycle, 0xFFFFU ) * signal_period ) / 0xFFFFU );
            break;

        case _PWM_TIM_AUX_0:
            signal_period = LL_TIM_GetAutoReload( TIM2 ) + 1;
            LL_TIM_OC_SetCompareCH1( TIM2, ( MIN( duty_cycle, 0xFFFFU ) * signal_period ) / 0xFFFFU );
            break;

        case _PWM_TIM_AUX_1:
            signal_period = LL_TIM_GetAutoReload( TIM12 ) + 1;
            LL_TIM_OC_SetCompareCH1( TIM12, ( MIN( duty_cycle, 0xFFFFU ) * signal_period ) / 0xFFFFU );
            break;

        case _PWM_TIM_AUX_2:
            signal_period = LL_TIM_GetAutoReload( TIM12 ) + 1;
            LL_TIM_OC_SetCompareCH2( TIM12, ( MIN( duty_cycle, 0xFFFFU ) * signal_period ) / 0xFFFFU );
            break;

        default:
            ASSERT( false );
            break;
    }
}

/* ----- End ---------------------------------------------------------------- */
