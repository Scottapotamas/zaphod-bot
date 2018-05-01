/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "hal_pwm.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "qassert.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE;

#define TIM9_PWM_PERIOD         1024

/* ----- Variables ---------------------------------------------------------- */

TIM_HandleTypeDef htim9;

/* ----- Private Functions -------------------------------------------------- */

PRIVATE void
hal_pwm_post_init( TIM_HandleTypeDef *htim );

/* ----- Public Functions --------------------------------------------------- */

/** Init TIM9 for PWM mode */

PUBLIC void
hal_pwm_init( uint32_t pwm_frequency_Hz )
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_OC_InitTypeDef     sConfigOC;

    /* Prescaler set to allow for around 20kHz PWM frequency which is the
     * maximum for this motor driver. */
    htim9.Instance           = TIM9;
    htim9.Init.CounterMode   = TIM_COUNTERMODE_UP;
    htim9.Init.Prescaler     = (uint32_t)( SystemCoreClock / ( pwm_frequency_Hz * TIM9_PWM_PERIOD ) );
    htim9.Init.Period        = TIM9_PWM_PERIOD;
    htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim9.State              = HAL_TIM_STATE_RESET;
    HAL_TIM_Base_Init( &htim9 );

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource( &htim9, &sClockSourceConfig );

    htim9.State              = HAL_TIM_STATE_RESET;
    HAL_TIM_PWM_Init( &htim9 );

    sConfigOC.OCMode       = TIM_OCMODE_PWM1;
    sConfigOC.Pulse        = 1;
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel( &htim9, &sConfigOC, TIM_CHANNEL_1 );
    HAL_TIM_PWM_ConfigChannel( &htim9, &sConfigOC, TIM_CHANNEL_2 );

    hal_pwm_post_init( &htim9 );
}

/* -------------------------------------------------------------------------- */

/** Stop and deinit TIM9 for when done with PWM mode */

PUBLIC void
hal_pwm_deinit( void )
{
    htim9.Instance = TIM9;
// TODO: Disabling this timer does not work correctly and we don't get any PWM outpuyt anymore!
    HAL_TIM_Base_Stop(&htim9);
    HAL_TIM_Base_DeInit(&htim9);
}

/* -------------------------------------------------------------------------- */

/** Start PWM on the indicated channel */

PUBLIC void
hal_pwm_start( HalPwmChannel_t channel )
{
    ASSERT( htim9.State != HAL_TIM_STATE_RESET );

    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1; /* 0% to start */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel( &htim9,
                               &sConfigOC,
                               ( channel == HAL_PWM_CHANNEL_1 )
                               ? TIM_CHANNEL_1 : TIM_CHANNEL_2  );
    HAL_TIM_PWM_Start( &htim9, ( channel == HAL_PWM_CHANNEL_1 )
                               ? TIM_CHANNEL_1 : TIM_CHANNEL_2  );
}

/* -------------------------------------------------------------------------- */

/** Set PWM on the indicated channel to the indicated percentage */

PUBLIC void
hal_pwm_set( HalPwmChannel_t channel, uint8_t percentage )
{
    ASSERT( htim9.State != HAL_TIM_STATE_RESET );

    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = ( MIN( percentage, 100U ) * TIM9_PWM_PERIOD ) / 100U;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel( &htim9,
                               &sConfigOC,
                               ( channel == HAL_PWM_CHANNEL_1 )
                               ? TIM_CHANNEL_1 : TIM_CHANNEL_2  );
    HAL_TIM_PWM_Start( &htim9, ( channel == HAL_PWM_CHANNEL_1 )
                               ? TIM_CHANNEL_1 : TIM_CHANNEL_2  );
}

/* -------------------------------------------------------------------------- */

/** Stop PWM on the indicated channel */

PUBLIC void
hal_pwm_stop( HalPwmChannel_t channel )
{
    //ASSERT( htim9.State != HAL_TIM_STATE_RESET );

    HAL_TIM_PWM_Stop( &htim9, ( channel == HAL_PWM_CHANNEL_1 )
                              ? TIM_CHANNEL_1 : TIM_CHANNEL_2 );
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if( tim_baseHandle->Instance==TIM9 )
    {
        /* Peripheral clock enable */
        __HAL_RCC_TIM9_CLK_ENABLE();
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_pwm_post_init( TIM_HandleTypeDef* tim_baseHandle )
{
    if( tim_baseHandle->Instance==TIM9 )
    {
        /** TIM9 GPIO Configuration
          * PE5     ------> TIM9_CH1
          * PE6     ------> TIM9_CH2
          */

        __HAL_RCC_GPIOE_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin       = 1;//M1_PWM_Pin | M2_PWM_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
HAL_TIM_Base_MspDeInit( TIM_HandleTypeDef* tim_baseHandle )
{
    if( tim_baseHandle->Instance == TIM9 )
    {
        GPIO_InitTypeDef GPIO_InitStruct;

        /** TIM9 GPIO Configuration
          *  PE5     ------> TIM9_CH1
          *  PE6     ------> TIM9_CH2
          */

        /* Switch pin back to output and force low */
        GPIO_InitStruct.Pin       = 1;//M1_PWM_Pin | M2_PWM_Pin;
        GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init( GPIOE, &GPIO_InitStruct );

        HAL_GPIO_WritePin( GPIOE,
                           1 | 2 ,
                           GPIO_PIN_RESET );
    }
}

/* ----- End ---------------------------------------------------------------- */
