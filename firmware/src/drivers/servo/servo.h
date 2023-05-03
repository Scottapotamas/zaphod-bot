#ifndef CLEARPATH_H
#define CLEARPATH_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "observer.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    _CLEARPATH_1 = 0,
    _CLEARPATH_2,
    _CLEARPATH_3,
    _CLEARPATH_4,
    _NUMBER_CLEARPATH_SERVOS
} ClearpathServoInstance_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_init( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void* servo_get_state_context_for( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC Observer * servo_get_observer( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_is_configured( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
servo_get_configured_count( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_change_configuration( ClearpathServoInstance_t servo,
                            bool is_installed,
                            bool requires_homing,
                            bool reverse_direction,
                            uint32_t steps_per_revolution,
                            float ratio,
                            float angle_min,
                            float angle_max,
                            float angle_at_home );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_enable( ClearpathServoInstance_t servo, bool enabled );

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_disable_all_hard( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_set_target_angle_limited( ClearpathServoInstance_t servo, float angle_degrees );

PUBLIC void
servo_set_target_angle_raw( ClearpathServoInstance_t servo, float angle_degrees );

PUBLIC float
servo_get_current_angle( ClearpathServoInstance_t servo );

PUBLIC uint16_t
servo_get_steps_per_second( ClearpathServoInstance_t servo );

PUBLIC float
servo_get_degrees_per_second( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_get_move_done( ClearpathServoInstance_t servo );

PUBLIC bool
servo_get_servo_ok( ClearpathServoInstance_t servo );

PUBLIC bool
servo_get_servo_did_error( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_task( void* arg );

/* -------------------------------------------------------------------------- */

#endif /* CLEARPATH_H */
