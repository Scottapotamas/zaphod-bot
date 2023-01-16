#ifndef CLEARPATH_H
#define CLEARPATH_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/* -------------------------------------------------------------------------- */

typedef enum
{
    _CLEARPATH_1 = 0,
    _CLEARPATH_2,
    _CLEARPATH_3,
    _CLEARPATH_4,
    _NUMBER_CLEARPATH_SERVOS
} ClearpathServoInstance_t;

/* ----- Types ------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
servo_init( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
servo_get_configured_count( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_start( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_stop( ClearpathServoInstance_t servo );

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

PUBLIC void
servo_process( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

#endif /* CLEARPATH_H */
