#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <electricui.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration_types.h"
#include "global.h"
#include "motion_types.h"
#include "user_interface_types.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
user_interface_init( void );

PUBLIC void
user_interface_handle_data( void );

PUBLIC bool
user_interface_connection_ok( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_reset_cause( const char *reset_description );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_assert_cause( const char *assert_description );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_report_error( char *error_string );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_cpu_load( uint8_t percent );

PUBLIC void
user_interface_set_cpu_clock( uint32_t clock );

PUBLIC void
user_interface_update_task_statistics( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_sensors_enabled( bool enable );

PUBLIC void
user_interface_set_module_enable( bool enable );

PUBLIC void
user_interface_set_input_voltage( float voltage );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_main_state( uint8_t state );

PUBLIC void
user_interface_set_control_mode( uint8_t mode );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_kinematics_mechanism_info( float shoulder_radius, float bicep_len, float forearm_len, float effector_radius );

PUBLIC void
user_interface_set_kinematics_limits( int32_t radius, int32_t zmin, int32_t zmax );

PUBLIC void
user_interface_set_kinematics_flips( int8_t x, int8_t y, int8_t z );

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
user_interface_get_fan_target( void );

PUBLIC void
user_interface_set_fan_percentage( uint8_t percent );

PUBLIC void
user_interface_set_fan_rpm( uint16_t rpm );

PUBLIC void
user_interface_set_fan_state( uint8_t state );

PUBLIC FanCurve_t *
user_interface_get_fan_curve_ptr( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_temp_ambient( float temp );

PUBLIC void
user_interface_set_temp_regulator( float temp );

PUBLIC void
user_interface_set_temp_external( float temp );

PUBLIC void
user_interface_set_temp_cpu( float temp );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_position( int32_t x, int32_t y, int32_t z );

PUBLIC void
user_interface_set_effector_speed( uint32_t microns_per_second );

PUBLIC CartesianPoint_t
user_interface_get_tracking_target();

PUBLIC void
user_interface_reset_tracking_target();

PUBLIC void
user_interface_set_movement_data( uint32_t sync_offset, uint8_t move_type, uint8_t progress );

PUBLIC void
user_interface_set_pathing_status( uint8_t status );

PUBLIC void
user_interface_set_motion_state( uint8_t status );

PUBLIC void
user_interface_set_motion_queue_depth( uint8_t utilisation );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_motor_enable( uint8_t servo, bool enable );

PUBLIC void
user_interface_motor_state( uint8_t servo, uint8_t state );

PUBLIC void
user_interface_motor_feedback( uint8_t servo, float percentage );

PUBLIC void
user_interface_motor_power( uint8_t servo, float watts );

PUBLIC void
user_interface_motor_speed( uint8_t servo, float degrees_per_second );

PUBLIC void
user_interface_motor_target_angle( uint8_t servo, float angle );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_led_status( uint8_t status );

PUBLIC void
user_interface_set_led_values( uint16_t red, uint16_t green, uint16_t blue );

PUBLIC void
user_interface_get_led_manual( float *r, float *g, float *b, uint8_t *en );

PUBLIC void
user_interface_set_led_queue_depth( uint8_t utilisation );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_push_position( void );

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif    // USER_INTERFACE_H
