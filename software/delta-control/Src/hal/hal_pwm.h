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
    HLFB_SERVO_1,
	HLFB_SERVO_2,
	HLFB_SERVO_3,
	HLFB_SERVO_4
} hlfb_timer;

typedef enum
{
    PWM_TIM_FAN,
	PWM_TIM_BUZZER,
	PWM_TIM_AUX_0,
	PWM_TIM_AUX_1,
	PWM_TIM_AUX_2
} pwm_output_timer;

/* ----- Public Functions -------------------------------------------------- */

PUBLIC void
hal_pwm_setup_hlfb( uint8_t servo_number );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_pwm_setup_ic(void);

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_pwm_setup_output(uint8_t pwm_output, uint16_t frequency, uint8_t duty_cycle);

/* -------------------------------------------------------------------------- */

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_PWM_H */
