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

PUBLIC bool
hal_hard_ic_valid( InputCaptureSignal_t input );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_hard_ic_read( InputCaptureSignal_t input );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_hard_ic_read_avg( InputCaptureSignal_t input );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
hal_hard_ic_read_peak( InputCaptureSignal_t input );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_HARD_IC_H */
