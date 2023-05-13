/* ----- System Includes ---------------------------------------------------- */

#include <float.h>
#include <math.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "cartesian_helpers.h"
#include "cartesian_interpolator.h"

#include "qassert.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC mm_per_second_t
cartesian_move_speed( Movement_t *movement )
{
    REQUIRE( movement->metadata.num_pts );

    // microns-per-millisecond converts to millimeters-per-second with no numeric conversion
    // long live the metric system
    return cartesian_move_distance( movement ) / movement->duration;
}

// Input speed is in centimeters/second
// Distance in microns
// Return the duration in milliseconds (round down)
PUBLIC uint32_t
cartesian_duration_for_speed( CartesianPoint_t *a, CartesianPoint_t *b, mm_per_second_t target_speed )
{
    REQUIRE( a );
    REQUIRE( b );

    uint32_t distance = cartesian_distance_between( a, b );    // in microns

    // 1 mm/second is 1 micron/millisecond
    return distance / target_speed;
}

// Input two points in 3D space (*a and *b), and 0.0f to 1.0f 'percentage' on the line to find
// Writes the resultant point into *p
PUBLIC void
cartesian_find_point_on_line( CartesianPoint_t *a, CartesianPoint_t *b, CartesianPoint_t *p, float weight )
{
    REQUIRE( a );
    REQUIRE( b );

    p->x = a->x + ( ( b->x - a->x ) * weight );
    p->y = a->y + ( ( b->y - a->y ) * weight );
    p->z = a->z + ( ( b->z - a->z ) * weight );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
cartesian_apply_rotation_offset( Movement_t *move, float offset_degrees )
{
    REQUIRE( move->metadata.num_pts );

    for( uint32_t i = 0; i < move->metadata.num_pts; i++ )
    {
        cartesian_point_rotate_around_z( &move->points[i], offset_degrees );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
cartesian_point_rotate_around_z( CartesianPoint_t *a, float degrees )
{
    REQUIRE( a );

    // Early exit if no rotation is required
    if( ( degrees <= 0.0f + FLT_EPSILON ) || ( degrees >= 360.0f - FLT_EPSILON ) )
    {
        return;
    }

    float radians = degrees * (float)M_PI / 180.0f;
    float cos_w   = cosf( radians );
    float sin_w   = sinf( radians );

    CartesianPoint_t rotated = { 0 };

    rotated.x = a->x * cos_w - a->y * sin_w;
    rotated.y = a->x * sin_w + a->y * cos_w;
    // a->z = a->z;     // we are rotating around z, so not needed

    a->x = rotated.x;
    a->y = rotated.y;
}

/* -------------------------------------------------------------------------- */

typedef struct
{
    float progress;
    uint32_t distance;
} SplineDistanceMap_t;

typedef struct
{
    uint32_t id;
    SplineDistanceMap_t lut[SPEED_SAMPLE_RESOLUTION+1];
} SplineMetadata;

SplineMetadata move_metadata = { 0 };

/* Apply calculations to find the 't' value to apply to a downstream pathing calculation
 * which will result 'linear' distance changes over the input progress [0.0f to 1.0f] percent.
 *
 * This uses the distance [microns] vs t [0.0f to 1.0f] lookup table generated during speed checks, and interpolates
 * between samples.
 */

PUBLIC float
cartesian_distance_linearisation_from_lut( uint32_t sync_offset, float progress )
{
    SplineMetadata *metadata = &move_metadata;
    float target_t = 0.0f;

    // Start or end positions don't need compensation
    if( progress <= 0.0f + FLT_EPSILON || progress >= 1.0f - FLT_EPSILON )
    {
        target_t = progress;
        return target_t;
    }

    // Don't attempt to compensate with mismatched LUT
    if( metadata->id != sync_offset )
    {
        target_t = progress;
        return target_t;
    }
    
    // Calculate the 'length' for this progress - between 0 and 'line length'
    uint32_t target_distance = (float)metadata->lut[SPEED_SAMPLE_RESOLUTION].distance * progress;

    // Zero distance moves don't need compensation
    if( target_distance <= 1 )
    {
        target_t = progress;
        return target_t;
    }

    // Walk through the lookup table until we find the pair of entries around our target length
    for( uint32_t i = 0; i <= SPEED_SAMPLE_RESOLUTION; i++ )
    {
        // Is the sum of line segments larger than the target length
        if( metadata->lut[i].distance >= target_distance )
        {
            SplineDistanceMap_t *lower = &metadata->lut[i - 1];
            SplineDistanceMap_t *upper = &metadata->lut[i];

            // Linearly interpolate with the previous entry to find the matching distance

            // let x represent progress, y represents the distance
            // 1 is 'lower', 2 is 'upper', 3 is 'interpolated
            // x3 = ( (y2-y3)*x1 + (y3-y1)*x2 ) / ( y2 - y1 )
            // x3 is therefore the value 't' we use when calculating positions on the spline
            float lerp_progress = ( (float)(upper->distance-target_distance)*lower->progress
                                    + (float)(target_distance-lower->distance)*upper->progress )
                                  / (float)( upper->distance - lower->distance );

            target_t = lerp_progress;
            break;
        }
    }

    return target_t;
}

// Calculate the distance of a movement
PUBLIC uint32_t
cartesian_move_distance( Movement_t *movement )
{
    REQUIRE( movement->metadata.num_pts );

    uint32_t distance = 0;
    MotionAdjective_t move_type = (MotionAdjective_t)movement->metadata.type;

    if( move_type == _POINT_TRANSIT || move_type == _LINE )
    {
        // straight line 3D distance
        distance = cartesian_distance_between( &movement->points[0], &movement->points[1] );
    }
    else
    {
        uint32_t         distance_sum   = 0;
        CartesianPoint_t sample_point   = { 0, 0, 0 };
        CartesianPoint_t previous_point = { 0, 0, 0 };

        // Copy the curve start point as the previous point, as first sample is non-zero
        switch( move_type )
        {
            case _BEZIER_QUADRATIC:
            case _BEZIER_QUADRATIC_LINEARISED:
                memcpy( &previous_point, &movement->points[_QUADRATIC_START], sizeof( CartesianPoint_t ) );
                break;

            case _BEZIER_CUBIC:
            case _BEZIER_CUBIC_LINEARISED:
                memcpy( &previous_point, &movement->points[_CUBIC_START], sizeof( CartesianPoint_t ) );
                break;

            case _CATMULL_SPLINE:
            case _CATMULL_SPLINE_LINEARISED:
                memcpy( &previous_point, &movement->points[_CATMULL_START], sizeof( CartesianPoint_t ) );
                break;

            case _POINT_TRANSIT:
            case _LINE:
                // these shouldn't be solved by slice-summation
                break;
        }

        // Copy the movements ID (sync timestamp) alongside metadata
        move_metadata.id = movement->sync_offset;

        // iteratively sum over a series of sampled positions
        for( uint32_t i = 1; i <= SPEED_SAMPLE_RESOLUTION; i++ )
        {
            // convert the step into a 0-1 float for 'percentage across line' input
            float sample_t = (float)i / SPEED_SAMPLE_RESOLUTION;

            // sample the position of the effector using the relevant interp processor
            switch( move_type )
            {
                case _BEZIER_QUADRATIC:
                case _BEZIER_QUADRATIC_LINEARISED:
                    cartesian_point_on_quadratic_bezier( movement->points, movement->metadata.num_pts, sample_t, &sample_point );
                    break;

                case _BEZIER_CUBIC:
                case _BEZIER_CUBIC_LINEARISED:
                    cartesian_point_on_cubic_bezier( movement->points, movement->metadata.num_pts, sample_t, &sample_point );
                    break;

                case _CATMULL_SPLINE:
                case _CATMULL_SPLINE_LINEARISED:
                    cartesian_point_on_catmull_spline( movement->points, movement->metadata.num_pts, sample_t, &sample_point );
                    break;

                case _POINT_TRANSIT:
                case _LINE:
                    // these shouldn't be solved by slice-summation
                    ASSERT_PRINTF(false, "Don't try to slice a line");
                    break;
            }

            // calculate distance between the previous sample and this sample
            uint32_t dist = cartesian_distance_between( &previous_point, &sample_point );

            // add to the running distance sum
            distance_sum += dist;

            // Add to the movement lookup table used for speed linearisation
            move_metadata.lut[i].progress = sample_t;
            move_metadata.lut[i].distance = distance_sum;

            // this sample will be used as the previous point in the next loop
            memcpy( &previous_point, &sample_point, sizeof( CartesianPoint_t ) );
        }

        distance = distance_sum;
    }


    return distance;
}

/* -------------------------------------------------------------------------- */

uint32_t cartesian_distance_between( CartesianPoint_t *a, CartesianPoint_t *b )
{
    REQUIRE( a );
    REQUIRE( b );

    uint32_t distance = 0;

    int32_t delta_x = a->x - b->x;
    int32_t delta_y = a->y - b->y;
    int32_t delta_z = a->z - b->z;
    float   dist    = sqrtf( ( (float)delta_x * delta_x ) + ( (float)delta_y * delta_y ) + ( (float)delta_z * delta_z ) );
    distance        = (uint32_t)fabsf( dist );

    return distance;
}

/* -------------------------------------------------------------------------- */
