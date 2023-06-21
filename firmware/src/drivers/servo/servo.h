#ifndef CLEARPATH_H
#define CLEARPATH_H

/* -------------------------------------------------------------------------- */

#include "global.h"

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

PUBLIC void servo_init( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void* servo_get_state_context_for( ClearpathServoInstance_t servo );

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

PUBLIC void
servo_disable_all_hard( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_set_target_angle_raw( ClearpathServoInstance_t servo, float angle_degrees );

PUBLIC float
servo_get_current_angle( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_task( void* arg );

/* -------------------------------------------------------------------------- */

#endif /* CLEARPATH_H */
