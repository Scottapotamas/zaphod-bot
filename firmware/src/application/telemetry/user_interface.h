#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "user_interface_types.h"
#include "movement_types.h"
#include "lighting_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
user_interface_init( void );

PUBLIC void
user_interface_task( void *arg );

/* -------------------------------------------------------------------------- */

typedef uint32_t (*MovementRequestFn)(Movement_t *move);

PUBLIC void user_interface_attach_motion_request_cb( MovementRequestFn callback );

/* -------------------------------------------------------------------------- */

typedef uint32_t (*LightingRequestFn)(Fade_t *move);

PUBLIC void user_interface_attach_lighting_request_cb( LightingRequestFn callback );

/* -------------------------------------------------------------------------- */

typedef void (*PositionRequestFn)(CartesianPoint_t *request);

PUBLIC void user_interface_attach_position_request_cb( PositionRequestFn callback );

/* -------------------------------------------------------------------------- */

typedef void (*HSIRequestFn)(HSIColour_t *request);

PUBLIC void user_interface_attach_colour_request_cb( HSIRequestFn callback );

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

PUBLIC void
user_interface_set_kinematics_mechanism_info( float shoulder_radius, float bicep_len, float forearm_len, float effector_radius );

PUBLIC void
user_interface_set_kinematics_limits( int32_t radius, int32_t zmin, int32_t zmax );

PUBLIC void
user_interface_set_kinematics_flips( int8_t x, int8_t y, int8_t z );

/* -------------------------------------------------------------------------- */

//PUBLIC FanCurve_t *
//user_interface_get_fan_curve_ptr( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif    // USER_INTERFACE_H
