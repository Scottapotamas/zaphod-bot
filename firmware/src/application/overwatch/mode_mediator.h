#ifndef MODE_MEDIATOR_H
#define MODE_MEDIATOR_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "signals.h"

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_request_mode( ControlModes_t request );

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_armed( bool armed );

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_request_rehome( void );

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_set_is_homed( bool effector_at_home );

/* -------------------------------------------------------------------------- */

PUBLIC ControlModes_t mode_mediator_get_mode( void );

/* -------------------------------------------------------------------------- */

PUBLIC void mode_mediator_task( void );

/* -------------------------------------------------------------------------- */

#endif    // MODE_MEDIATOR_H