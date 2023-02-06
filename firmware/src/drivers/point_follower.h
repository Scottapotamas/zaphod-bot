#ifndef POINT_FOLLOWER_H
#define POINT_FOLLOWER_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include <motion_types.h>

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    POINT_FOLLOWER_DELTA = 0,
    POINT_FOLLOWER_EXPANSION,
    NUMBER_POINT_FOLLOWERS
} PointFollowInstance_t;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
point_follower_init( PointFollowInstance_t follower );

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_process_delta( void );

PUBLIC void
point_follower_process_expansion( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_process( PointFollowInstance_t follower );

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_set_target( PointFollowInstance_t follower, CartesianPoint_t *target );

/* -------------------------------------------------------------------------- */

PUBLIC bool
point_follower_get_move_done( PointFollowInstance_t follower );

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_start( PointFollowInstance_t follower );

/* -------------------------------------------------------------------------- */

PUBLIC void
point_follower_stop( PointFollowInstance_t follower );

/* -------------------------------------------------------------------------- */

#endif /* POINT_FOLLOWER_H */
