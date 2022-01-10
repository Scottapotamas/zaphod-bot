#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "configuration_types.h"

/* ----- Defines ------------------------------------------------------------ */


/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
configuration_init( void );

PUBLIC void
configuration_set_defaults( void );

PUBLIC void
configuration_load( void );

PUBLIC void
configuration_save( void );

/* -------------------------------------------------------------------------- */

PUBLIC FanCurve_t *
configuration_get_fan_curve_ptr( void );

/* -------------------------------------------------------------------------- */

PUBLIC int16_t
configuration_get_voltage_trim_mV( void );

PUBLIC int16_t
configuration_get_servo_trim_mA( uint8_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void *
configuration_set_led_correction_ptr( LedSettings_t *ptr );

PUBLIC void
configuration_get_led_whitebalance( uint16_t *red_offset, uint16_t *green_offset, uint16_t *blue_offset );

PUBLIC void
configuration_get_led_bias( uint16_t *offset );

PUBLIC bool
configuration_get_led_luma_correction_enabled( void );

PUBLIC bool
configuration_get_led_wb_correction_enabled( void );


/* -------------------------------------------------------------------------- */

PUBLIC float
configuration_get_rotation_z();

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_H */
