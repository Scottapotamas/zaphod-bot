#ifndef HAL_POWER_H
#define HAL_POWER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

/** Returns converted voltage from input adc reading
  */
PUBLIC float
hal_voltage_V( uint32_t raw_adc );

/* -------------------------------------------------------------------------- */

PUBLIC float
hal_current_A( uint32_t raw_adc );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_POWER_H */
