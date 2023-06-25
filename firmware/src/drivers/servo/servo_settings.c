/* -------------------------------------------------------------------------- */

#include <float.h>  // needed for FLT_EPSILON

#include "servo_settings.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

typedef struct
{
    bool installed;
    bool requires_homing;
    bool reverse_direction;
    uint32_t steps_per_revolution;
    float ratio;
    float angle_min;
    float angle_max;
    float angle_at_home;
} ServoConfiguration_t;

PRIVATE ServoConfiguration_t clearpath[_NUMBER_CLEARPATH_SERVOS];

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
                            float angle_at_home )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];

    // TODO: consider doing bounds/error checks on these values?
    me->installed            = is_installed;
    me->requires_homing      = requires_homing;
    me->reverse_direction    = reverse_direction;
    me->steps_per_revolution = steps_per_revolution;
    me->ratio                = ratio;
    me->angle_min            = angle_min;
    me->angle_max            = angle_max;
    me->angle_at_home        = angle_at_home;

    return true;
}

/* -------------------------------------------------------------------------- */

PUBLIC void servo_load_configuration( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];

    switch( servo )
    {
        case _CLEARPATH_1:
        case _CLEARPATH_2:
        case _CLEARPATH_3:
            me->installed            = true;
            me->requires_homing      = true;
            me->reverse_direction    = false;
            me->steps_per_revolution = SERVO_STEPS_PER_REV;
            me->ratio                = 1.0f;
            me->angle_min            = -1.0f*SERVO_MIN_ANGLE;
            me->angle_max            = SERVO_MAX_ANGLE;
            me->angle_at_home        = -42.0f;
            break;

        case _CLEARPATH_4:
            me->installed            = false;
            me->requires_homing      = false;
            me->reverse_direction    = false;
            me->steps_per_revolution = 0;
            me->ratio                = 0;
            me->angle_min            = 0;
            me->angle_max            = 0;
            me->angle_at_home        = 0;
            break;

        default:
            ASSERT(false);
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_is_configured( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];

    return ( me->installed && me->ratio >= 0.0f + FLT_EPSILON );
}

/* -------------------------------------------------------------------------- */

// Returns the number of servos that have valid configuration
//  implies that this number servos are ready for use
PUBLIC uint8_t servo_get_configured_count( void )
{
    uint8_t num_configured = 0;

    for( ClearpathServoInstance_t servo = 0; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
    {
        num_configured += servo_is_configured(servo);
    }

    return num_configured;
}

/* -------------------------------------------------------------------------- */

PUBLIC float servo_config_get_home_angle( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];

    return me->angle_at_home;
}

PUBLIC bool servo_config_requires_homing( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];

    return me->requires_homing;
}

PUBLIC bool servo_config_direction_reversed( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];

    return me->reverse_direction;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_angle_within_limits( ClearpathServoInstance_t servo, float angle )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];

    return (angle > me->angle_min && angle < me->angle_max);
}

/* -------------------------------------------------------------------------- */

/*
 * The kinematics output is an angle between -85 and +90, where 0 deg is when
 * the elbow-shaft link is parallel to the frame plate. Full range not available due
 * to physical limits on arm travel. Approx -45 to +70 is the safe working range.
 *
 * Servo steps are referenced to the homing point, which is the HW minimum,
 * therefore we need to convert the angle into steps, and apply the adequate offset.
 * The servo's range is approx 2300 counts.
 */
PUBLIC int32_t servo_angle_to_steps( ClearpathServoInstance_t servo, float angle )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];
    REQUIRE( me->ratio > 0.0f + FLT_EPSILON );

    float steps_per_servo_degree = (float)me->steps_per_revolution / SERVO_ANGLE_PER_REV;
    float output_ratio = me->ratio;

    float converted_angle  = angle * output_ratio * steps_per_servo_degree;
    return (int32_t)converted_angle;
}

/* -------------------------------------------------------------------------- */

/*
 * Convert a motor position in steps to an angle in the motor reference frame
 */
PUBLIC float servo_steps_to_angle( ClearpathServoInstance_t servo, int32_t steps )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoConfiguration_t *me = &clearpath[servo];
    REQUIRE( me->ratio > 0.0f + FLT_EPSILON );

    float steps_per_degree = (float)me->steps_per_revolution / SERVO_ANGLE_PER_REV;
    float output_ratio = me->ratio;

    float steps_to_angle = (float)steps / steps_per_degree / output_ratio;
    return steps_to_angle;
}

/* -------------------------------------------------------------------------- */
