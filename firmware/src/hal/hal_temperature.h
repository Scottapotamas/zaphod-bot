#ifndef HAL_TEMPERATURE_H
#define HAL_TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

/** Return a ADC sample of the internal temperature sensor converted in
  * tents of degrees C. Note the function can return values below 0 degrees!
  */
PUBLIC float
hal_temperature_micro_degrees_C( uint32_t raw_adc );

/* -------------------------------------------------------------------------- */

PUBLIC float
hal_temperature_pcb_degrees_C( uint32_t raw_adc );

/* -------------------------------------------------------------------------- */

PUBLIC float
hal_temperature_ext_degrees_C( uint32_t raw_adc );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_TEMPERATURE_H */
