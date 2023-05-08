#ifndef PATH_INTERPOLATOR_H
#define PATH_INTERPOLATOR_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "movement_types.h"

/* -------------------------------------------------------------------------- */

//! The callback function which path_interpolator will call with a request position
typedef void (*PositionRequestFn)(CartesianPoint_t *position);

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_update_output_callback( PositionRequestFn callback );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_process( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_set_epoch_reference( uint32_t sync_timer );

/* -------------------------------------------------------------------------- */

PUBLIC void path_interpolator_add_request( Movement_t *movement_to_process );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t path_interpolator_queue_ready( void );

/* -------------------------------------------------------------------------- */

PUBLIC float
path_interpolator_get_progress( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
path_interpolator_get_move_done( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_start( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
path_interpolator_stop( void );

/* -------------------------------------------------------------------------- */

#endif /* PATH_INTERPOLATOR_H */