#ifndef HAL_IC_HARD_H
#define HAL_IC_HARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    HAL_IC_HARD_HLFB_SERVO_1,
    HAL_IC_HARD_HLFB_SERVO_2,
    HAL_IC_HARD_HLFB_SERVO_3,
    HAL_IC_HARD_HLFB_SERVO_4,
    HAL_IC_HARD_FAN_HALL,
    HAL_IC_HARD_NUM
} InputCaptureSignal_t;

typedef void (*HardICDataCallback)(InputCaptureSignal_t channel, uint32_t value);

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_ic_hard_init( HardICDataCallback callback );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_setup_capture( uint8_t input );

/* -------------------------------------------------------------------------- */

void TIM8_CC_IRQHandler( void );
void TIM3_IRQHandler( void );
void TIM4_IRQHandler( void );
void TIM1_CC_IRQHandler( void );
void TIM5_IRQHandler( void );
void TIM1_BRK_TIM9_IRQHandler( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_IC_HARD_H */
