#ifndef EFFECTOR_H
#define EFFECTOR_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "cartesian_types.h"
#include "subject.h"

/* -------------------------------------------------------------------------- */

PUBLIC void effector_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC Subject * effector_get_subject( void );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t effector_get_speed( void );

/* -------------------------------------------------------------------------- */

PUBLIC void effector_request_target( CartesianPoint_t *position );

/* -------------------------------------------------------------------------- */

PUBLIC void effector_set_home( void );

/* -------------------------------------------------------------------------- */

PUBLIC CartesianPoint_t effector_get_position( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool effector_is_near_home( void );

/* -------------------------------------------------------------------------- */

PUBLIC void effector_task( void* arg );

/* -------------------------------------------------------------------------- */

#endif /* EFFECTOR_H */
