/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "random_points.h"
#include "qassert.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

PRIVATE float dirty_random_float( void );

PRIVATE CartesianPoint_t previous_position = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC void
random_points_init( void )
{
    memset( &previous_position, 0, sizeof(CartesianPoint_t) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
random_points_get_move( uint8_t sequence_index, Movement_t *move )
{
    REQUIRE( move );

    // Generate a (valid) 3D position randomly
    //   we'll also randomly generate the duration to that point
    Movement_t random_transit = { 0 };

    random_transit.ref = _POS_ABSOLUTE;
    random_transit.type = _LINE;
    random_transit.num_pts = 2;

    // Moving from the previous end position
    random_transit.points[_LINE_START].x = previous_position.x;
    random_transit.points[_LINE_START].y = previous_position.y;
    random_transit.points[_LINE_START].z = previous_position.z;

    // Random point between x|y -150 to +150, z +50 to +200
    random_transit.points[_LINE_END].x = (( dirty_random_float() - 0.5f ) * 300);
    random_transit.points[_LINE_END].y = (( dirty_random_float() - 0.5f ) * 300);
    random_transit.points[_LINE_END].z = 50 + ( dirty_random_float() * 150 ) ;

    // No shorter than 100ms, up to 2100ms
    random_transit.duration = 100 + (dirty_random_float() * 2000 );

    memcpy( &previous_position, &random_transit.points[_LINE_END], sizeof( CartesianPoint_t ) );

    // Return a 'eased' linear transit
    // Convert our linear movement into a fancy shaped move
    cartesian_plan_smoothed_line( &random_transit, 0.005f, 0.005f );
}

/* -------------------------------------------------------------------------- */

// Returns a low quality 'random' float between 0.0 and 1.0
PRIVATE float dirty_random_float( void )
{
    float normalised = (float)rand() / (float)RAND_MAX; // between 0.0 and 1.0
    return normalised;
}

/* ----- End ---------------------------------------------------------------- */