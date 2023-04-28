#ifndef HAL_PWM_H
#define HAL_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    _PWM_TIM_FAN,
    _PWM_TIM_BUZZER,
    _PWM_TIM_AUX_0,
    _PWM_TIM_AUX_1,
    _PWM_TIM_AUX_2,
    _PWM_NUMBER_TIMERS
} PWMOutputTimerDef_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_pwm_generation( PWMOutputTimerDef_t pwm_output, uint16_t frequency );

/* -------------------------------------------------------------------------- */

PUBLIC
void hal_pwm_set_percentage_f( PWMOutputTimerDef_t pwm_output, float percentage );

/* -------------------------------------------------------------------------- */

PUBLIC
void hal_pwm_set_byte( PWMOutputTimerDef_t pwm_output, uint8_t duty_cycle );

/* -------------------------------------------------------------------------- */

PRIVATE void
hal_pwm_set( PWMOutputTimerDef_t pwm_output, uint16_t duty_cycle );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_PWM_H */
