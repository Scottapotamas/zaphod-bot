#ifndef EFFECTOR_H
#define EFFECTOR_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include <motion_types.h>

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types ------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
effector_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t
effector_get_speed( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_request_target( CartesianPoint_t *position );

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_set_home( void );

/* -------------------------------------------------------------------------- */

PUBLIC CartesianPoint_t
effector_get_position( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
effector_process( void );

/* -------------------------------------------------------------------------- */

#endif /* EFFECTOR_H */