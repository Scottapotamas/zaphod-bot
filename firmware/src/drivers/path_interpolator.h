#ifndef PATH_INTERPOLATOR_H
#define PATH_INTERPOLATOR_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "timer_ms.h"
#include <motion_types.h>

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    PATH_INTERPOLATOR_DELTA = 0,
    PATH_INTERPOLATOR_EXPANSION,
    NUMBER_PATH_INTERPOLATORS
} PathInterpolatorInstance_t;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
path_interpolator_init( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process_delta( void );

PUBLIC void
path_interpolator_process_expansion( void );


/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_epoch_reference( PathInterpolatorInstance_t interpolator,
                                       timer_ms_t sync_timer );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_next( PathInterpolatorInstance_t interpolator,
                            Movement_t *movement_to_process );

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_is_ready_for_next( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

PUBLIC float
path_interpolator_get_progress( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_get_move_done( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_start( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_stop( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

#endif /* PATH_INTERPOLATOR_H */
