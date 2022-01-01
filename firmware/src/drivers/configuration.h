#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration_types.h"
#include "global.h"

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

PUBLIC void
configuration_notify_fan_curve( void );

/* -------------------------------------------------------------------------- */

PUBLIC UserConfig_t *
configuration_get_user_config_ptr( void );

PUBLIC void
configuration_notify_config( void );

PUBLIC uint16_t
configuration_get_effector_speed_limit( void );

PUBLIC float
configuration_get_z_rotation( void );

/* -------------------------------------------------------------------------- */

PUBLIC PowerCalibration_t *
configuration_get_power_calibration_ptr( void );

PUBLIC int16_t
configuration_get_voltage_trim_mV( void );

PUBLIC int16_t
configuration_get_servo_trim_mA( uint8_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC LedSettings_t *
configuration_get_led_calibration_ptr( void );

PUBLIC void
configuration_get_led_whitebalance( uint16_t *red_offset, uint16_t *green_offset, uint16_t *blue_offset );

PUBLIC void
configuration_get_led_bias( uint16_t *offset );

PUBLIC bool
configuration_get_led_luma_correction_enabled( void );

PUBLIC bool
configuration_get_led_wb_correction_enabled( void );

/* -------------------------------------------------------------------------- */

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_H */
