#ifndef PATH_INTERPOLATOR_H
#define PATH_INTERPOLATOR_H

/* ----- Local Includes ----------------------------------------------------- */

#include <motion_types.h>
#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types ------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
kinematics_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC KinematicsSolution_t
path_lerp_line( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output );

/* -------------------------------------------------------------------------- */

PUBLIC KinematicsSolution_t
path_catmull_spline( CartesianPoint_t p[], size_t points, float pos_weight, CartesianPoint_t *output );

/* -------------------------------------------------------------------------- */

#endif /* PATH_INTERPOLATOR_H */
