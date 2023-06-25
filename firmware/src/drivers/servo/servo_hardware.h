#ifndef SERVO_HARDWARE_H
#define SERVO_HARDWARE_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "servo_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC void servo_hw_disable_all( void );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_hw_enable( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_hw_disable( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_hw_oc_fault( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_hw_step( ClearpathServoInstance_t servo,
                           bool direction,
                           uint32_t steps );

/* -------------------------------------------------------------------------- */

PUBLIC void servo_load_hardware_defaults( ClearpathServoInstance_t servo );

/* -------------------------------------------------------------------------- */

#endif    // SERVO_HARDWARE_H