#include "cartesian_interpolator.h"
#include "cartesian_helpers.h"
#include "qassert.h"

#include <float.h>
#include <math.h>
#include <string.h>

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */


// Calculate the control points for a cubic Bézier curve which allows for acceleration shaping of a linear move
// Accepts a line based movement, and mutates it into a cubic bezier
// Should be called BEFORE sending the movement to the queue
PUBLIC MotionSolution_t
cartesian_plan_smoothed_line( Movement_t *movement, float start_weight, float end_weight )
{
    REQUIRE( movement->metadata.num_pts );

    // Error checks - only accept lines with 2 points
    if( (MotionAdjective_t)movement->metadata.type != _LINE || movement->metadata.num_pts != 2 )
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
    movement->metadata.type    = _BEZIER_CUBIC;
    movement->metadata.num_pts = 4;

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1] are the two points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the line
// the output pointer is the interpolated position on the line

PUBLIC MotionSolution_t
cartesian_point_on_line( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    REQUIRE( p );
    REQUIRE( points == 2 );

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
    REQUIRE( p );
    REQUIRE( points == 4 );

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
    REQUIRE( p );
    REQUIRE( points == 3 );

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
    REQUIRE( p );
    REQUIRE( points == 4 );

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
    REQUIRE( p );
    REQUIRE( points == 2 ); // need 2 points for quadratic solution?

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
    float a           = 1.0f / (float)numSpirals;
    float denominator = sqrtf( 1.0f + a * a * t * t );

    output->x = cosf( t ) / denominator;
    output->y = sinf( t ) / denominator;
    output->z = ( -1.0f * a * t ) / denominator;

    return SOLUTION_VALID;
}

/* ----- End ---------------------------------------------------------------- */
