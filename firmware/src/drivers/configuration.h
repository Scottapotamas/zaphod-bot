#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

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

PUBLIC int16_t
configuration_get_voltage_trim_mV( void );

PUBLIC int16_t
configuration_get_servo_trim_mA( uint8_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_get_led_whitebalance( int16_t *red_offset, int16_t *green_offset, int16_t *blue_offset );

PUBLIC void
configuration_get_led_bias( int16_t *offset );

/* -------------------------------------------------------------------------- */

PUBLIC float
configuration_get_rotation_z();

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_H */
