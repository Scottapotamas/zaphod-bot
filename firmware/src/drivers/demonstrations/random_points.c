/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "random_points.h"
#include "qassert.h"
#include "hal_systick.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

PRIVATE float dirty_random_float( void );

PRIVATE CartesianPoint_t previous_position = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC void
random_points_init( void )
{
    memset( &previous_position, 0, sizeof(CartesianPoint_t) );
    srand( hal_systick_get_ms() );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
random_points_get_move( uint8_t sequence_index, Movement_t *move )
{
    REQUIRE( move );

    // Generate a (valid) 3D position randomly
    //   we'll also randomly generate the duration to that point
    Movement_t random_transit = { 0 };

    random_transit.metadata.ref = _POS_ABSOLUTE;
    random_transit.metadata.type = _LINE;
    random_transit.metadata.num_pts = 2;

    // Moving from the previous end position
    random_transit.points[_LINE_START].x = previous_position.x;
    random_transit.points[_LINE_START].y = previous_position.y;
    random_transit.points[_LINE_START].z = previous_position.z;

    // Random point between x|y -150 to +150, z +100 to +200
    random_transit.points[_LINE_END].x = MM_TO_MICRONS( (dirty_random_float() - 0.5f) * 300 );
    random_transit.points[_LINE_END].y = MM_TO_MICRONS( (dirty_random_float() - 0.5f) * 300 );
    random_transit.points[_LINE_END].z = MM_TO_MICRONS( 100 + (dirty_random_float() * 100) );

    // Random duration up to 2000ms
    uint32_t rand_duration = dirty_random_float() * 2000;
    uint32_t min_duration = cartesian_duration_for_speed( &random_transit.points[_LINE_START], &random_transit.points[_LINE_END], 400 );
    random_transit.duration = CLAMP( rand_duration, min_duration, 5000 );

    memcpy( &previous_position, &random_transit.points[_LINE_END], sizeof( CartesianPoint_t ) );

    // Return a 'eased' linear transit
    // Convert our linear movement into a fancy shaped move
    cartesian_plan_smoothed_line( &random_transit, 0.005f, 0.005f );
    memcpy( move, &random_transit, sizeof( Movement_t ) );
}

/* -------------------------------------------------------------------------- */

// Returns a low quality 'random' float between 0.0 and 1.0
PRIVATE float dirty_random_float( void )
{
    float normalised = (float)rand() / (float)RAND_MAX; // between 0.0 and 1.0
    return normalised;
}

/* ----- End ---------------------------------------------------------------- */