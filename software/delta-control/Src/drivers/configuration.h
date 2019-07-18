#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "motion_types.h"

/* ----- Defines ------------------------------------------------------------ */

typedef enum {
	PIN_INACTIVE = 0,
	PIN_INPUT,
	PIN_ADC,
	PIN_DAC,
	PIN_OUTPUT,
	PIN_PWM,
	PIN_UART,
	PIN_I2C,
	PIN_CAN
} ConfigurableIOMode_t;

typedef struct
{
	uint8_t temperature;
	uint8_t percentage;
} FanCurve_t;

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
config_set_cpu_load( uint8_t percent );

PUBLIC void
config_set_cpu_clock( float clock );

PUBLIC void
config_set_cpu_temp( float temp );

PUBLIC void
config_sensors_enable( bool enable );

PUBLIC void
config_module_enable( bool enable );

PUBLIC void
config_set_input_voltage( float voltage );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_main_state( uint8_t state );

PUBLIC void
config_set_control_mode( uint8_t mode );


/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_fan_percentage( uint8_t percent );

PUBLIC void
config_set_fan_rpm( uint16_t rpm );

PUBLIC void
config_set_fan_state( uint8_t state );

PUBLIC FanCurve_t*
config_get_fan_curve_ptr( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_temp_ambient( float temp );

PUBLIC void
config_set_temp_regulator( float temp );

PUBLIC void
config_set_temp_external( float temp );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_position( int32_t x, int32_t y, int32_t z );

PUBLIC CartesianPoint_t
config_get_tracking_target();

PUBLIC void
config_set_movement_data( uint8_t move_id, uint8_t move_type, uint8_t progress );

PUBLIC void
config_set_pathing_status( uint8_t status );

PUBLIC void
config_set_motion_state( uint8_t status );

PUBLIC void
config_set_motion_queue_depth( uint8_t utilisation );


/* -------------------------------------------------------------------------- */

PUBLIC void
config_motor_enable( uint8_t servo, bool enable );

PUBLIC void
config_motor_state( uint8_t servo, uint8_t state );

PUBLIC void
config_motor_feedback( uint8_t servo, uint8_t fb );

PUBLIC void
config_motor_power( uint8_t servo, float watts );

PUBLIC void
config_motor_target_angle( uint8_t servo, float angle );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_led_status( uint8_t status );

PUBLIC void
config_set_led_values(uint8_t red, uint8_t green, uint8_t blue);

PUBLIC void
config_set_led_queue_depth( uint8_t utilisation );


/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_H */
