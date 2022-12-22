/* ----- System Includes ---------------------------------------------------- */

#include <float.h>
#include <math.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "app_times.h"
#include "motion_types.h"

/* ----- Defines ------------------------------------------------------------ */

/* -------------------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC mm_per_second_t
cartesian_move_speed( Movement_t *movement )
{
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
    uint32_t distance = cartesian_distance_between( a, b );    // in microns

    // 1 mm/second is 1 micron/millisecond
    return distance / target_speed;
}

// Input two points in 3D space (*a and *b), and 0.0f to 1.0f 'percentage' on the line to find
// Writes the resultant point into *p
PUBLIC void
cartesian_find_point_on_line( CartesianPoint_t *a, CartesianPoint_t *b, CartesianPoint_t *p, float weight )
{
    p->x = a->x + ( ( b->x - a->x ) * weight );
    p->y = a->y + ( ( b->y - a->y ) * weight );
    p->z = a->z + ( ( b->z - a->z ) * weight );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
cartesian_point_rotate_around_z( CartesianPoint_t *a, float degrees )
{
    float radians = degrees * M_PI / 180.0f;
    float cos_w   = cosf( radians );
    float sin_w   = sinf( radians );

    a->x = a->x * cos_w - a->y * sin_w;
    a->y = a->x * sin_w + a->y * cos_w;
    // a->z = a->z;     // we are rotating around z, so not needed
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
    uint32_t distance = 0;

    if( movement )
    {
        if( movement->type == _POINT_TRANSIT || movement->type == _LINE )
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
            switch( movement->type )
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
                switch( movement->type )
                {
                    case _BEZIER_QUADRATIC:
                    case _BEZIER_QUADRATIC_LINEARISED:
                        cartesian_point_on_quadratic_bezier( movement->points, movement->num_pts, sample_t, &sample_point );
                        break;

                    case _BEZIER_CUBIC:
                    case _BEZIER_CUBIC_LINEARISED:
                        cartesian_point_on_cubic_bezier( movement->points, movement->num_pts, sample_t, &sample_point );
                        break;

                    case _CATMULL_SPLINE:
                    case _CATMULL_SPLINE_LINEARISED:
                        cartesian_point_on_catmull_spline( movement->points, movement->num_pts, sample_t, &sample_point );
                        break;

                    case _POINT_TRANSIT:
                    case _LINE:
                        // these shouldn't be solved by slice-summation
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
    }

    return distance;
}

/* -------------------------------------------------------------------------- */

uint32_t cartesian_distance_between( CartesianPoint_t *a, CartesianPoint_t *b )
{
    int32_t distance = 0;

    // pointer null checks
    if( a && b )
    {
        int32_t delta_x = a->x - b->x;
        int32_t delta_y = a->y - b->y;
        int32_t delta_z = a->z - b->z;
        float   dist    = sqrtf( ( (float)delta_x * delta_x ) + ( (float)delta_y * delta_y ) + ( (float)delta_z * delta_z ) );
        distance        = (int32_t)fabsf( dist );
    }

    return distance;
}

/* -------------------------------------------------------------------------- */

// Calculate the control points for a cubic Bézier curve which allows for acceleration shaping of a linear move
// Accepts a line based movement, and mutates it into a cubic bezier
// Should be called BEFORE sending the movement to the queue
PUBLIC MotionSolution_t
cartesian_plan_smoothed_line( Movement_t *movement, float start_weight, float end_weight )
{
    // Error checks - only accept lines with 2 points
    if( movement->type != _LINE || movement->num_pts != 2 )
    {
        // Error
        return SOLUTION_ERROR;
    }

    // Move the move destination to the last point
    memcpy( &movement->points[_CUBIC_END], &movement->points[_LINE_END], sizeof( CartesianPoint_t ) );

    // Create control points on the line between the start and end points of the line,
    // the weight defines how close the controls are to the start/end points -> how aggressive the acceleration shaping will be
    // smaller weight value -> smaller distance between the (start and control1) and (control2 and end), therefore slower acceleration
    cartesian_find_point_on_line( &movement->points[_CUBIC_START], &movement->points[_CUBIC_END], &movement->points[_CUBIC_CONTROL_A], start_weight );
    cartesian_find_point_on_line( &movement->points[_CUBIC_END], &movement->points[_CUBIC_START], &movement->points[_CUBIC_CONTROL_B], end_weight );

    // "Fix" the other fields in the movement to match our new movement
    movement->type    = _BEZIER_CUBIC;
    movement->num_pts = 4;

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1] are the two points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the line
// the output pointer is the interpolated position on the line

PUBLIC MotionSolution_t
cartesian_point_on_line( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if( points < 2 )
    {
        // need 2 points for a line
        return SOLUTION_ERROR;
    }

    // start and end of splines don't need calculation
    if( pos_weight <= 0.0f + FLT_EPSILON )
    {
        memcpy( output, &p[_LINE_START], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    if( pos_weight >= 1.0f - FLT_EPSILON )
    {
        memcpy( output, &p[_LINE_END], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    // Linear interpolation between two points (lerp)
    output->x = p[_LINE_START].x + pos_weight * ( p[_LINE_END].x - p[_LINE_START].x );
    output->y = p[_LINE_START].y + pos_weight * ( p[_LINE_END].y - p[_LINE_START].y );
    output->z = p[_LINE_START].z + pos_weight * ( p[_LINE_END].z - p[_LINE_START].z );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2], p[3] are the control points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p1 and p2
// the output pointer is the interpolated position on the curve between p1 and p2

PUBLIC MotionSolution_t
cartesian_point_on_catmull_spline( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if( points < 4 )
    {
        // need 4 points for solution
        return SOLUTION_ERROR;
    }

    // start and end of splines don't need calculation as catmull curves _will_ pass through all points
    if( pos_weight <= 0.0f + FLT_EPSILON )
    {
        memcpy( output, &p[_CATMULL_START], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;    // todo add a 'end of range' flag?
    }

    if( pos_weight >= 1.0f - FLT_EPSILON )
    {
        memcpy( output, &p[_CATMULL_END], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    /* Derivation from http://www.mvps.org/directx/articles/catmull/
     *
                                    [  0  2  0  0 ]   [ p0 ]
        q(t) = 0.5( t, t^2, t^3 ) * | -1  0  1  0 | * | p1 |
                                    |  2 -5  4 -1 |   | p2 |
                                    [ -1  3 -3  1 ]   [ p3 ]
     */

    // pre-calculate
    float t  = pos_weight;
    float t2 = t * t;
    float t3 = t2 * t;

    output->x = 0.5f * ( ( 2 * p[_CATMULL_START].x ) + ( -p[_CATMULL_CONTROL_A].x + p[_CATMULL_END].x ) * t + ( 2 * p[_CATMULL_CONTROL_A].x - 5 * p[_CATMULL_START].x + 4 * p[_CATMULL_END].x - p[_CATMULL_CONTROL_B].x ) * t2 + ( -p[_CATMULL_CONTROL_A].x + 3 * p[_CATMULL_START].x - 3 * p[_CATMULL_END].x + p[_CATMULL_CONTROL_B].x ) * t3 );

    output->y = 0.5f * ( ( 2 * p[_CATMULL_START].y ) + ( -p[_CATMULL_CONTROL_A].y + p[_CATMULL_END].y ) * t + ( 2 * p[_CATMULL_CONTROL_A].y - 5 * p[_CATMULL_START].y + 4 * p[_CATMULL_END].y - p[_CATMULL_CONTROL_B].y ) * t2 + ( -p[_CATMULL_CONTROL_A].y + 3 * p[_CATMULL_START].y - 3 * p[_CATMULL_END].y + p[_CATMULL_CONTROL_B].y ) * t3 );

    output->z = 0.5f * ( ( 2 * p[_CATMULL_START].z ) + ( -p[_CATMULL_CONTROL_A].z + p[_CATMULL_END].z ) * t + ( 2 * p[_CATMULL_CONTROL_A].z - 5 * p[_CATMULL_START].z + 4 * p[_CATMULL_END].z - p[_CATMULL_CONTROL_B].z ) * t2 + ( -p[_CATMULL_CONTROL_A].z + 3 * p[_CATMULL_START].z - 3 * p[_CATMULL_END].z + p[_CATMULL_CONTROL_B].z ) * t3 );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2] are the start, control and end points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p0 and p2
// the output pointer is the interpolated position on the curve between p0 and p2

PUBLIC MotionSolution_t
cartesian_point_on_quadratic_bezier( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if( points < 3 )
    {
        // need 3 points for quadratic solution
        return SOLUTION_ERROR;
    }

    // start and end of bezier
    if( pos_weight <= 0.0f + FLT_EPSILON )
    {
        memcpy( output, &p[_QUADRATIC_START], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    if( pos_weight >= 1.0f - FLT_EPSILON )
    {
        memcpy( output, &p[_QUADRATIC_END], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    // General form for a quadratic bezier curve

    // B(t) = ((1-t)^2 * p0) + (2(1 - t) * t * p1) + (t^2 * p2) where 0 < t < 1

    // cache oft-used values to improve read-ability
    float t   = pos_weight;
    float tsq = t * t;

    float omt  = 1 - t;
    float omt2 = omt * omt;

    output->x = ( omt2 * p[_QUADRATIC_START].x ) + ( 2 * omt * t * p[_QUADRATIC_CONTROL].x ) + ( tsq * p[_QUADRATIC_END].x );
    output->y = ( omt2 * p[_QUADRATIC_START].y ) + ( 2 * omt * t * p[_QUADRATIC_CONTROL].y ) + ( tsq * p[_QUADRATIC_END].y );
    output->z = ( omt2 * p[_QUADRATIC_START].z ) + ( 2 * omt * t * p[_QUADRATIC_CONTROL].z ) + ( tsq * p[_QUADRATIC_END].z );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2], p[3] are the start, control, control and end points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p0 and p2
// the output pointer is the interpolated position on the curve between p0 and p2

PUBLIC MotionSolution_t
cartesian_point_on_cubic_bezier( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if( points < 4 )
    {
        // need 4 points for cubic solution
        return SOLUTION_ERROR;
    }

    // start and end of bezier
    if( pos_weight <= 0.0f + FLT_EPSILON )
    {
        memcpy( output, &p[_CUBIC_START], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    if( pos_weight >= 1.0f - FLT_EPSILON )
    {
        memcpy( output, &p[_CUBIC_END], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    // General form for a cubic bezier curve

    // B(t) = ((1-t)^3 * p0) + (3(1 - t)^2 * t * P1) + (3(1-t)t^2 * P2) + (t^3 * P3) where 0 < t < 1

    // cache oft-used values to improve read-ability
    float t   = pos_weight;
    float tsq = t * t;
    float tcu = tsq * t;

    float omt  = 1 - t;
    float omt2 = omt * omt;
    float omt3 = omt2 * omt;

    output->x = ( omt3 * p[_CUBIC_START].x ) + ( 3 * omt2 * t * p[_CUBIC_CONTROL_A].x ) + ( 3 * omt * tsq * p[_CUBIC_CONTROL_B].x ) + ( tcu * p[_CUBIC_END].x );
    output->y = ( omt3 * p[_CUBIC_START].y ) + ( 3 * omt2 * t * p[_CUBIC_CONTROL_A].y ) + ( 3 * omt * tsq * p[_CUBIC_CONTROL_B].y ) + ( tcu * p[_CUBIC_END].y );
    output->z = ( omt3 * p[_CUBIC_START].z ) + ( 3 * omt2 * t * p[_CUBIC_CONTROL_A].z ) + ( 3 * omt * tsq * p[_CUBIC_CONTROL_B].z ) + ( tcu * p[_CUBIC_END].z );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1] are the start and end points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p0 and p1
// the output pointer is the interpolated position on the curve between p0 and p1
// https://blender.stackexchange.com/questions/42131/modelling-a-spiral-around-a-sphere/42159

PUBLIC MotionSolution_t
cartesian_point_on_spiral( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if( points < 1 )
    {
        // need 2 points for quadratic solution
        return SOLUTION_ERROR;
    }

    // start and end of the helix
    if( pos_weight <= 0.0f + FLT_EPSILON )
    {
        memcpy( output, &p[0], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    if( pos_weight >= 1.0f - FLT_EPSILON )
    {
        memcpy( output, &p[1], sizeof( CartesianPoint_t ) );
        return SOLUTION_VALID;
    }

    // General form for a spherical spiral loxodrome (parametric equations)

    //  x(t) = cos(t) / sqrt( 1 + a^2 * t^2 )
    //	y(t) = sin(t) / sqrt( 1 + a^2 * t^2 )
    //	z(t) = - a*t  / sqrt( 1 + a^2 * t^2 )
    //  where 0 < t < 1, r is the winding radius

    uint8_t numSpirals = 7;

    // cache oft-used values to improve read-ability
    float t           = pos_weight;
    float a           = 1.0f / numSpirals;
    float denominator = sqrtf( 1.0f + a * a * t * t );

    output->x = cosf( t ) / denominator;
    output->y = sinf( t ) / denominator;
    output->z = ( -1.0f * a * t ) / denominator;

    return SOLUTION_VALID;
}

/* ----- End ---------------------------------------------------------------- */
