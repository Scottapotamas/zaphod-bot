#ifndef PATH_INTERPOLATOR_H
#define PATH_INTERPOLATOR_H

/* -------------------------------------------------------------------------- */

#include "global.h"

#include "movement_types.h"
#include "subject.h"

/* -------------------------------------------------------------------------- */

//! The callback function which path_interpolator will call with a request position
typedef void (*PositionRequestFn)(CartesianPoint_t *position);

/* -------------------------------------------------------------------------- */

PUBLIC void path_interpolator_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC Subject * path_interpolator_get_subject( void );

/* -------------------------------------------------------------------------- */

PUBLIC void path_interpolator_update_output_callback( PositionRequestFn callback );

/* -------------------------------------------------------------------------- */

PUBLIC void path_interpolator_task( void* arg );

/* -------------------------------------------------------------------------- */

PUBLIC void path_interpolator_set_epoch_reference( uint32_t sync_timer );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t path_interpolator_queue_request( Movement_t *movement_to_process );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t path_interpolator_queue_ready( void );

/* -------------------------------------------------------------------------- */

PUBLIC float path_interpolator_get_progress( void );

/* -------------------------------------------------------------------------- */

PUBLIC void path_interpolator_update_effector_position( int32_t effector_x,
                                                        int32_t effector_y,
                                                        int32_t effector_z );

/* -------------------------------------------------------------------------- */

PUBLIC bool path_interpolator_get_move_done( void );

/* -------------------------------------------------------------------------- */

#endif /* PATH_INTERPOLATOR_H */