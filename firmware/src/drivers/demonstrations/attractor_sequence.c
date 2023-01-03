/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "attractor_sequence.h"
#include "attractor_types.h"
#include "app_times.h"

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_sequence_init( void )
{

}

/* -------------------------------------------------------------------------- */

/*
 * Move creation process
 *
 * If we're not at home,
 *  Grab the current position
 * Otherwise
 *  Generate a random position to start from
 *
 * */

PRIVATE float simulation_time = 0.0f;

PUBLIC void
attractor_sequence_get_move( uint8_t index, Movement_t *move )
{
    AttractorPosition_t samples[4] = { 0 };

    // Sample the strange attractor for positions
    // TODO: cache previous ones so subsequent moves don't recalculate the ones used by the sliding window
    for( uint8_t sample = 0; sample < 4; sample++ )
    {
        attractor_calc_point_at_t( simulation_time + ((float)sample * 0.1f), LORENZ, &samples[sample] );
    }

    // TODO time shift should be handled more elegantly than this
    simulation_time += 0.5f;

    // Create a sliding catmull rom spline out of the sampled points
    Movement_t sliding_catmull = { 0 };

    sliding_catmull.type = _CATMULL_SPLINE_LINEARISED;
    sliding_catmull.ref = _POS_ABSOLUTE;

    memcpy( &sliding_catmull.points[_CATMULL_CONTROL_A], &samples[0], sizeof(CartesianPoint_t) );
    memcpy( &sliding_catmull.points[_CATMULL_START],     &samples[1], sizeof(CartesianPoint_t) );
    memcpy( &sliding_catmull.points[_CATMULL_END],       &samples[2], sizeof(CartesianPoint_t) );
    memcpy( &sliding_catmull.points[_CATMULL_CONTROL_B], &samples[3], sizeof(CartesianPoint_t) );

    sliding_catmull.num_pts = 4;

    // Calculate the move duration in order to satisfy the target velocity
    // Distance estimate is coarse (i.e. linear approximation)
    uint32_t proposed_duration = cartesian_duration_for_speed( &sliding_catmull.points[_CATMULL_START],
                                                               &sliding_catmull.points[_CATMULL_END],
                                                               ATTRACTOR_DEFAULT_SPEED );

    // Enforce a minimum move duration
    sliding_catmull.duration = MAX( proposed_duration, ATTRACTOR_MINIMUM_DURATION_MS );

    // TODO: sync-offset handling is required

    memcpy( move, &sliding_catmull, sizeof(Movement_t) );
}

/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */


/* ----- End ---------------------------------------------------------------- */
