#ifndef PATH_INTERPOLATOR_H
#define PATH_INTERPOLATOR_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "movement_types.h"

/* -------------------------------------------------------------------------- */


typedef enum
{
    PATH_INTERPOLATOR_DELTA = 0,
    PATH_INTERPOLATOR_EXPANSION,
    NUMBER_PATH_INTERPOLATORS
} PathInterpolatorInstance_t;

//! The callback function which path_interpolator will call with a request position
typedef void (*PositionRequestFn)(CartesianPoint_t *position);

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_init( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_update_output_callback( PositionRequestFn callback );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process_delta( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process( PathInterpolatorInstance_t interpolator );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_epoch_reference( PathInterpolatorInstance_t interpolator,
                                       uint32_t sync_timer );

/* -------------------------------------------------------------------------- */

PUBLIC void path_interpolator_add_request( Movement_t *movement_to_process );

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