#ifndef SENSORS_H
#define SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "hal_adc.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Public Functions --------------------------------------------------- */

/* Optionally init any required hardware */

PUBLIC void
sensors_init( void );

/* -------------------------------------------------------------------------- */

/** Enable adc for each channel */

PUBLIC void
sensors_enable( void );

/* -------------------------------------------------------------------------- */

/** Disable adc for relevant channels */

PUBLIC void
sensors_disable( void );

/* -------------------------------------------------------------------------- */

/** Return averaged and normalised temperature readings */

PUBLIC float
sensors_ambient_C( void );

PUBLIC float
sensors_12v_regulator_C( void );

PUBLIC float
sensors_expansion_C( void );

PUBLIC float
sensors_microcontroller_C( void );

/* -------------------------------------------------------------------------- */

PUBLIC float
sensors_input_V( void );

PUBLIC float
sensors_servo_A( HalAdcInput_t servo_to_sample );

PUBLIC float
sensors_servo_W( HalAdcInput_t servo_to_sample );

/* -------------------------------------------------------------------------- */



/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* SENSORS_H */
