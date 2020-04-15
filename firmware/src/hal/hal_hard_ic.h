#ifndef HAL_HARD_IC_H
#define HAL_HARD_IC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    HAL_HARD_IC_HLFB_SERVO_1,
    HAL_HARD_IC_HLFB_SERVO_2,
    HAL_HARD_IC_HLFB_SERVO_3,
    HAL_HARD_IC_HLFB_SERVO_4,
    HAL_HARD_IC_FAN_HALL,
    HAL_HARD_IC_NUM
} InputCaptureSignal_t;

/* ----- Public Functions -------------------------------------------------- */

PUBLIC void
hal_hard_ic_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_setup_capture( uint8_t input );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_hard_ic_read( InputCaptureSignal_t input );

/* -------------------------------------------------------------------------- */

PUBLIC float
hal_hard_ic_read_f( InputCaptureSignal_t input );

/* -------------------------------------------------------------------------- */

void TIM8_CC_IRQHandler( void );
void TIM3_IRQHandler( void );
void TIM4_IRQHandler( void );
void TIM1_CC_IRQHandler( void );
void TIM5_IRQHandler( void );
void TIM1_BRK_TIM9_IRQHandler( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_HARD_IC_H */
