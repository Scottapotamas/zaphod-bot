#ifndef PATH_INTERPOLATOR_H
#define PATH_INTERPOLATOR_H

/* ----- Local Includes ----------------------------------------------------- */

#include <motion_types.h>
#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types ------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
path_interpolator_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_next(Movement_t	* movement_to_process );

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_is_ready_for_next( void );

/* -------------------------------------------------------------------------- */

PUBLIC float
path_interpolator_get_progress( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_get_move_done( void );

/* -------------------------------------------------------------------------- */

PUBLIC CartesianPoint_t
path_interpolator_get_global_position( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_start(void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_stop(void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_home( void );

/* -------------------------------------------------------------------------- */

#endif /* PATH_INTERPOLATOR_H */
