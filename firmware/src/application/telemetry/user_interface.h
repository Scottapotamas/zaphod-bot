#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "user_interface_types.h"
#include "movement_types.h"

#include "observer.h"
#include "subject.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_init( void );

PUBLIC void
user_interface_task( void *arg );

/* -------------------------------------------------------------------------- */

PUBLIC Observer * user_interface_get_observer( void );

/* -------------------------------------------------------------------------- */

PUBLIC Subject * user_interface_get_request_subject( void );

/* -------------------------------------------------------------------------- */

typedef void (*MovementRequestFn)(Movement_t *move);

PUBLIC void user_interface_attach_motion_request_cb( MovementRequestFn callback );

/* -------------------------------------------------------------------------- */

typedef void (*PositionRequestFn)(CartesianPoint_t *request);

PUBLIC void user_interface_attach_position_request_cb( PositionRequestFn callback );

/* -------------------------------------------------------------------------- */

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


/* -------------------------------------------------------------------------- */


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
user_interface_set_fan_state( uint8_t state );

//PUBLIC FanCurve_t *
//user_interface_get_fan_curve_ptr( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_motion_queue_depth( uint8_t utilisation );

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_set_led_status( uint8_t status );

PUBLIC void
user_interface_set_led_values( uint16_t red, uint16_t green, uint16_t blue );

PUBLIC void
user_interface_set_led_queue_depth( uint8_t utilisation );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif    // USER_INTERFACE_H
