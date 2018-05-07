#ifndef CLEARPATH_H
#define CLEARPATH_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/* -------------------------------------------------------------------------- */

typedef enum
{
	_CLEARPATH_1,
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

PUBLIC void
servo_run_startup( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_set_target_angle( ClearpathServoInstance_t servo, float angle_degrees );

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_process( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

#endif /* CLEARPATH_H */
