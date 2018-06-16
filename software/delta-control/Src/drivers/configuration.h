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

PUBLIC void
configuration_electric_setup( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_pub_cpu_load( uint8_t percent );

PUBLIC void
config_pub_cpu_clock( float clock );

PUBLIC void
config_pub_cpu_temp( float temp );

PUBLIC void
config_sensors_enable( bool enable );

PUBLIC void
config_module_enable( bool enable );

PUBLIC void
config_pub_input_voltage( float voltage );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_pub_fan_percentage( uint8_t percent );

PUBLIC void
config_pub_fan_rpm( uint16_t rpm );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_pub_temp_ambient( float temp );

PUBLIC void
config_pub_temp_regulator( float temp );

PUBLIC void
config_pub_temp_external( float temp );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_pub_position( int32_t x, int32_t y, int32_t z );

PUBLIC void
config_pub_movement_data( uint8_t move_type, uint8_t progress );

//todo add higher level movement mode?

/* -------------------------------------------------------------------------- */

PUBLIC void
config_motor_enable( uint8_t servo, bool enable );

PUBLIC void
config_motor_in_motion( uint8_t servo, bool moving );

PUBLIC void
config_motor_error( uint8_t servo, bool isError );

PUBLIC void
config_motor_power( uint8_t servo, float watts );

PUBLIC void
config_motor_current( uint8_t servo, float amps );

PUBLIC void
config_motor_angle( uint8_t servo, float angle );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_H */
