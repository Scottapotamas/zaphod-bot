#ifndef HAL_SOFT_IC_H
#define HAL_SOFT_IC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
	HAL_SOFT_IC_HALL = 0,
	HAL_SOFT_IC_NUM
} HalSoftICInput_t;

/* ------------------------- Functions Prototypes --------------------------- */

PUBLIC void
hal_soft_ic_init( void );

/* -------------------------------------------------------------------------- */

/** Return true when there are active samples in the averaging buffer */

PUBLIC bool
hal_soft_ic_valid( HalSoftICInput_t input );

/* -------------------------------------------------------------------------- */

/** Last converted read */

PUBLIC uint32_t
hal_soft_ic_read( HalSoftICInput_t input );

/* -------------------------------------------------------------------------- */

/** Averaged read */

PUBLIC uint32_t
hal_soft_ic_read_avg( HalSoftICInput_t input );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_soft_ic_start( HalSoftICInput_t input, uint16_t poll_rate_ms );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_soft_ic_stop( HalSoftICInput_t input );

/* -------------------------------------------------------------------------- */

/** Timer tick to trigger a sample */

PUBLIC void
hal_soft_ic_tick( void );

/* ------------------------- End -------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_SOFT_IC_H */
