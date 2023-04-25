#ifndef HAL_IC_SOFT_H
#define HAL_IC_SOFT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    HAL_IC_SOFT_HALL = 0,
    HAL_IC_SOFT_NUM
} HalSoftICInput_t;

/* ------------------------- Functions Prototypes --------------------------- */

PUBLIC void
hal_ic_soft_init( void );

/* -------------------------------------------------------------------------- */

/** Return true when there are active samples in the averaging buffer */

PUBLIC bool
hal_ic_soft_valid( HalSoftICInput_t input );

/* -------------------------------------------------------------------------- */

/** Last converted read */

PUBLIC uint32_t
hal_ic_soft_read( HalSoftICInput_t input );

/* -------------------------------------------------------------------------- */

/** Averaged read */

PUBLIC uint32_t
hal_ic_soft_read_avg( HalSoftICInput_t input );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_ic_soft_start( HalSoftICInput_t input, uint16_t poll_rate_ms );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_ic_soft_stop( HalSoftICInput_t input );

/* -------------------------------------------------------------------------- */

/** Timer tick to trigger a sample */

PUBLIC void
hal_ic_soft_tick( void );

/* ------------------------- End -------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_IC_SOFT_H */
