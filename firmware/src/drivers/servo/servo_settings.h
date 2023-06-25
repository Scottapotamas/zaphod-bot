#ifndef SERVO_SETTINGS_H
#define SERVO_SETTINGS_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "servo_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_change_configuration( ClearpathServoInstance_t servo,
                                        bool is_installed,
                                        bool requires_homing,
                                        bool reverse_direction,
                                        uint32_t steps_per_revolution,
                                        float ratio,
                                        float angle_min,
                                        float angle_max,
                                        float angle_at_home );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_load_configuration( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_is_configured( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t servo_get_configured_count( void );

/* -------------------------------------------------------------------------- */

PUBLIC float servo_config_get_home_angle( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_config_requires_homing( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_config_direction_reversed( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_angle_within_limits( ClearpathServoInstance_t servo,
                                       float angle );

/* -------------------------------------------------------------------------- */

PUBLIC int32_t servo_angle_to_steps( ClearpathServoInstance_t servo,
                                     float angle );

/* -------------------------------------------------------------------------- */

PUBLIC float servo_steps_to_angle( ClearpathServoInstance_t servo,
                                   int32_t steps );

/* -------------------------------------------------------------------------- */

#endif    // SERVO_SETTINGS_H