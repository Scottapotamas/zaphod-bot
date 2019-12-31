#ifndef HAL_HARD_IC_H
#define HAL_HARD_IC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Types ------------------------------------------------------------- */

//todo Cleanup HLFBinput to include the hall sensor input, general purpose PWM input (make it a more generic name etc
typedef enum
{
    _HLFB_SERVO_1,
	_HLFB_SERVO_2,
	_HLFB_SERVO_3,
	_HLFB_SERVO_4,
	_FAN_HALL,
	_HLFB_SERVO_NUM
} HLFBTimerDef_t;

typedef struct
{
	//todo update comments with correct units
    uint16_t	frequency;	//frequency in
    uint16_t	duty;		//duty cycle as
} PWMInputData_t;

/* ----- Public Functions -------------------------------------------------- */

PUBLIC void
hal_setup_capture(uint8_t input);

/* -------------------------------------------------------------------------- */

PUBLIC void
debug_get_ic();

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_pwm_capture( uint8_t servo_number );

/* -------------------------------------------------------------------------- */


/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_HARD_IC_H */
