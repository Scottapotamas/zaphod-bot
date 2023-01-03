/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "attractor_sequence.h"
#include "attractor_types.h"
#include "app_times.h"

/* -------------------------------------------------------------------------- */

PRIVATE void
attractor_sequence_randomise_start( AttractorPosition_t *output );

/* -------------------------------------------------------------------------- */

PRIVATE AttractorPosition_t cache[4] = { 0 };
PRIVATE bool cache_ok = false;
PRIVATE uint8_t selected_attractor = 0;

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_sequence_init( uint8_t species )
{
    selected_attractor = species;
    memset( &cache, 0, sizeof(AttractorPosition_t) * 4 );
    cache_ok = false;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_sequence_get_move( uint8_t index, Movement_t *move )
{
    AttractorPosition_t candidates[4] = { 0 };
    CartesianPoint_t    points[4]     = { 0 };

    AttractorSystem_t *settings = attractor_get_settings_ptr( selected_attractor );

    if( cache_ok )
    {
        // Slide the cache into the new positions for this section, and generate a new final part
        AttractorPosition_t new_point = { 0 };
        attractor_runge_kutta( settings, cache[_CATMULL_CONTROL_B], &new_point );

        memcpy( &candidates[_CATMULL_CONTROL_A], &cache[_CATMULL_START],     sizeof(AttractorPosition_t) );
        memcpy( &candidates[_CATMULL_START],     &cache[_CATMULL_END],       sizeof(AttractorPosition_t) );
        memcpy( &candidates[_CATMULL_END],       &cache[_CATMULL_CONTROL_B], sizeof(AttractorPosition_t) );
        memcpy( &candidates[_CATMULL_CONTROL_B], &new_point,                 sizeof(AttractorPosition_t) );
    }
    else // Generate fresh points
    {
        AttractorPosition_t random_start = { 0 };
        attractor_sequence_randomise_start( &random_start );

        attractor_runge_kutta( settings, random_start,                   &candidates[_CATMULL_CONTROL_A] );
        attractor_runge_kutta( settings, candidates[_CATMULL_CONTROL_A], &candidates[_CATMULL_START] );
        attractor_runge_kutta( settings, candidates[_CATMULL_START],     &candidates[_CATMULL_END] );
        attractor_runge_kutta( settings, candidates[_CATMULL_END],       &candidates[_CATMULL_CONTROL_B] );
    }

    // Copy the results into the cache for next iteration
    memcpy( &cache, &candidates, sizeof(AttractorPosition_t) * 4 );
    cache_ok = true;

    // Convert floating point positions into natural cartesian space (integer microns)
    // Also offset z as the attractors generally orbit z = 0
    for( uint8_t sample = 0; sample < 4; sample++ )
    {
        points[sample].x = MM_TO_MICRONS( candidates[sample].x );
        points[sample].y = MM_TO_MICRONS( candidates[sample].y );
        points[sample].z = MM_TO_MICRONS( candidates[sample].z ) + MM_TO_MICRONS(100);
    }

    // Create a sliding catmull rom spline out of the sampled points
    Movement_t sliding_catmull = { 0 };

    sliding_catmull.type = _CATMULL_SPLINE;
    sliding_catmull.ref = _POS_ABSOLUTE;

    memcpy( &sliding_catmull.points[_CATMULL_CONTROL_A], &points[0], sizeof(CartesianPoint_t) );
    memcpy( &sliding_catmull.points[_CATMULL_START],     &points[1], sizeof(CartesianPoint_t) );
    memcpy( &sliding_catmull.points[_CATMULL_END],       &points[2], sizeof(CartesianPoint_t) );
    memcpy( &sliding_catmull.points[_CATMULL_CONTROL_B], &points[3], sizeof(CartesianPoint_t) );

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

// Not intended to actually be random, it's used for a starting position only
PRIVATE float dirty_random_float( void )
{
    float normalised = (float)rand() / (float)RAND_MAX; // between 0.0 and 1.0
    return ( normalised - 0.5f ) * 10.0f;
}

// Generate a random position for the strange attractor to start at
PRIVATE void
attractor_sequence_randomise_start( AttractorPosition_t *output )
{
    if( output )
    {
        output->x = dirty_random_float();
        output->y = dirty_random_float();
        output->z = dirty_random_float();
    }
}

/* -------------------------------------------------------------------------- */


/* ----- End ---------------------------------------------------------------- */
