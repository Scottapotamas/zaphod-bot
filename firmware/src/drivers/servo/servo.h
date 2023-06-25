#ifndef CLEARPATH_H
#define CLEARPATH_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "servo_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC void servo_init( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void* servo_get_state_context_for( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_task( void* arg );

/* -------------------------------------------------------------------------- */

PUBLIC float servo_get_current_angle( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

#endif /* CLEARPATH_H */
