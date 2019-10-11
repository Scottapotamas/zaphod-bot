#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include <src/electricui.h>
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

typedef enum {
    CONTROL_NONE = 0,
    CONTROL_MANUAL,
    CONTROL_EVENT,
    CONTROL_TRACK,
    CONTROL_DEMO,
    CONTROL_CHANGING,
} ControlModes_t;

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

PUBLIC void
configuration_eui_callback( uint8_t link, eui_interface_t *interface, uint8_t message );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_report_error( char * error_string );

/* -------------------------------------------------------------------------- */

PUBLIC void
config_set_cpu_load( uint8_t percent );

PUBLIC void
config_set_cpu_clock(uint32_t clock);

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
config_set_kinematics_mechanism_info( float shoulder_radius, float bicep_len, float forearm_len, float effector_radius );

PUBLIC void
config_set_kinematics_limits( int32_t xmin, int32_t xmax, int32_t ymin, int32_t ymax, int32_t zmin, int32_t zmax );

PUBLIC void
config_set_kinematics_flips( int8_t x, int8_t y, int8_t z );

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
config_reset_tracking_target();

PUBLIC void
config_set_movement_data( uint8_t move_id, uint8_t move_type, uint8_t progress );

PUBLIC void
config_set_pathing_status( uint8_t status );

PUBLIC void
config_set_motion_state( uint8_t status );

PUBLIC void
config_set_motion_queue_depth( uint8_t utilisation );

PUBLIC float
config_get_rotation_z();

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
config_set_led_values(uint16_t red, uint16_t green, uint16_t blue);

PUBLIC void
config_set_led_queue_depth( uint8_t utilisation );

PUBLIC void
config_get_led_whitebalance( int16_t *red_offset, int16_t *green_offset, int16_t *blue_offset );

PUBLIC void
config_get_led_bias( int16_t *offset );


/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* CONFIGURATION_H */
