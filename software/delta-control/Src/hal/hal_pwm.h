#ifndef HAL_PWM_H
#define HAL_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    _HLFB_SERVO_1,
	_HLFB_SERVO_2,
	_HLFB_SERVO_3,
	_HLFB_SERVO_4,
	_HLFB_SERVO_NUM
} HLFBTimerDef_t;

typedef enum
{
    _PWM_TIM_FAN,
	_PWM_TIM_BUZZER,
	_PWM_TIM_AUX_0,
	_PWM_TIM_AUX_1,
	_PWM_TIM_AUX_2,
	_PWM_NUMBER_TIMERS
} PWMOutputTimerDef_t;


/* ----- Public Functions -------------------------------------------------- */

PUBLIC void
hal_pwm_setup_hlfb( uint8_t servo_number );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_pwm_setup_ic(void);

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_pwm_setup(PWMOutputTimerDef_t pwm_output, uint16_t frequency, uint8_t duty_cycle);

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_pwm_set(PWMOutputTimerDef_t pwm_output, uint8_t duty_cycle);

/* -------------------------------------------------------------------------- */

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_PWM_H */
