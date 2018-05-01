#ifndef HAL_PWM_H
#define HAL_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    HAL_PWM_CHANNEL_1,
    HAL_PWM_CHANNEL_2,
} HalPwmChannel_t;

/* ----- Public Functions -------------------------------------------------- */

/** Init TIM9 for PWM mode */

PUBLIC void
hal_pwm_init( uint32_t pwm_frequency_Hz );

/* -------------------------------------------------------------------------- */

/** Stop and deinit TIM9 for when done with PWM mode */

PUBLIC void
hal_pwm_deinit( void );

/* -------------------------------------------------------------------------- */

/** Start PWM on the indicatd channel */

PUBLIC void
hal_pwm_start( HalPwmChannel_t channel );

/* -------------------------------------------------------------------------- */

/** Set PWM on the indicatd channel to the indicated percentage */

PUBLIC void
hal_pwm_set( HalPwmChannel_t channel, uint8_t percentage );

/* -------------------------------------------------------------------------- */

/** Stop PWM on the indicatd channel */

PUBLIC void
hal_pwm_stop( HalPwmChannel_t channel );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_PWM_H */
