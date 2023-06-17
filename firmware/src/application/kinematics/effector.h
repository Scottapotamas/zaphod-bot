#ifndef EFFECTOR_H
#define EFFECTOR_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "cartesian_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC void effector_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void effector_request_target( CartesianPoint_t *position );

/* -------------------------------------------------------------------------- */

PUBLIC void effector_reset( void );

/* -------------------------------------------------------------------------- */

PUBLIC void effector_task( void* arg );

/* -------------------------------------------------------------------------- */

#endif /* EFFECTOR_H */
