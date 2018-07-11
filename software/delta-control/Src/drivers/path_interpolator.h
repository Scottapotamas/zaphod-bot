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
path_interpolator_set_objective( Movement_t	* movement_to_process );

/* -------------------------------------------------------------------------- */

PUBLIC float
path_interpolator_get_progress( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_get_move_done( void );

/* -------------------------------------------------------------------------- */

#endif /* PATH_INTERPOLATOR_H */
