#ifndef POINT_FOLLOWER_H
#define POINT_FOLLOWER_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "movement_types.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types ------------------------------------------------------------- */

typedef void (*PositionRequestFn)(CartesianPoint_t *position);

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
point_follower_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_update_output_callback( PositionRequestFn callback );

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_task( void* arg );

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_set_target( CartesianPoint_t *target );

/* -------------------------------------------------------------------------- */

#endif /* POINT_FOLLOWER_H */
