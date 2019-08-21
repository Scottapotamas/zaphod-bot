/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "motion_types.h"
#include "app_times.h"

/* ----- Defines ------------------------------------------------------------ */



/* -------------------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC int32_t
cartesian_move_speed(Movement_t *movement)
{
    // microns-per-millisecond converts to centimeters-per-second with simple div10
    // long live the metric system
    return (cartesian_move_distance(movement) / movement->duration ) / 10;
}

/* -------------------------------------------------------------------------- */

// Calculate the distance of a movement
PUBLIC int32_t
cartesian_move_distance(Movement_t *movement)
{
    int32_t distance = 0;

    if( movement )
    {
        if( movement->ref == _POINT_TRANSIT || movement->ref == _LINE )
        {
            // straight line 3D distance
            distance = cartesian_distance_between(&movement->points[0], &movement->points[1]);
        }
        else
        {
            uint32_t distance_sum = 0;
            CartesianPoint_t sample_point = { 0, 0, 0 };
            CartesianPoint_t previous_point = { 0, 0, 0 };

            // Copy the curve start point as the previous point, as first sample is non-zero
            memcpy( &previous_point, &movement->points[0], sizeof(CartesianPoint_t) );

            // iteratively sum over a series of sampled positions
            for (uint32_t i = 1; i < SPEED_SAMPLE_RESOLUTION; i++)
            {
                // convert the step into a 0-1 float for 'percentage across line' input
                float sample_t = i / SPEED_SAMPLE_RESOLUTION;

                // sample the position of the effector using the relevant interp processor
                switch( movement->ref )
                {
                    case _BEZIER_QUADRATIC:
                        cartesian_point_on_quadratic_bezier(movement->points, movement->num_pts, sample_t,
                                                            &sample_point);

                        break;

                    case _BEZIER_CUBIC:
                        cartesian_point_on_cubic_bezier(movement->points, movement->num_pts, sample_t, &sample_point);
                        break;

                    case _CATMULL_SPLINE:
                        cartesian_point_on_catmull_spline(movement->points, movement->num_pts, sample_t, &sample_point);
                        break;
                }

                // calculate distance between the previous sample and this sample
                int32_t dist = cartesian_distance_between( &previous_point, &sample_point );

                // add to the running distance sum
                distance_sum += dist;

                // this sample will be used as the previous point in the next loop
                memcpy( &previous_point, &sample_point, sizeof(CartesianPoint_t) );
            }
        }
    }

    return distance;
}
/* -------------------------------------------------------------------------- */

int32_t cartesian_distance_between(CartesianPoint_t *a, CartesianPoint_t *b)
{
    int32_t distance = 0;

    // pointer null checks
    if( a && b )
    {
        int32_t delta_x = a->x - b->x;
        int32_t delta_y = a->y - b->y;
        int32_t delta_z = a->z - b->z;
        float dist =  sqrtf( (delta_x*delta_x) + (delta_y*delta_y) + (delta_z*delta_z) ) ;
        distance = abs( (uint32_t)dist );
    }

    return distance;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1] are the two points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the line
// the output pointer is the interpolated position on the line

PUBLIC KinematicsSolution_t
cartesian_point_on_line( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if(points < 2)
    {
        // need 2 points for a line
        return SOLUTION_ERROR;
    }

    // start and end of splines don't need calculation
    if(pos_weight <= 0.0f + FLT_EPSILON)
    {
        memcpy( output, &p[0], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    if(pos_weight >= 1.0f - FLT_EPSILON)
    {
        memcpy( output, &p[0], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    // Linear interpolation between two points (lerp)
    output->x = p[0].x + pos_weight*( p[1].x - p[0].x );
    output->y = p[0].y + pos_weight*( p[1].y - p[0].y );
    output->z = p[0].z + pos_weight*( p[1].z - p[0].z );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2], p[3] are the control points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p1 and p2
// the output pointer is the interpolated position on the curve between p1 and p2

PUBLIC KinematicsSolution_t
cartesian_point_on_catmull_spline( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if(points < 4)
    {
        // need 4 points for solution
        return SOLUTION_ERROR;
    }

    // start and end of splines don't need calculation as catmull curves _will_ pass through all points
    if(pos_weight <= 0.0f + FLT_EPSILON)
    {
        memcpy( output, &p[1], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;	// todo add a 'end of range' flag?
    }

    if(pos_weight >= 1.0f - FLT_EPSILON)
    {
        memcpy( output, &p[2], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    /* Derivation from http://www.mvps.org/directx/articles/catmull/
     *
								[  0  2  0  0 ]   [ p0 ]
	q(t) = 0.5( t, t^2, t^3 ) * [ -1  0  1  0 ] * [ p1 ]
								[  2 -5  4 -1 ]   [ p2 ]
								[ -1  3 -3  1 ]   [ p3 ]
     */

    // pre-calculate
    float t = pos_weight;
    float t2 = t * t;
    float t3 = t2 * t;

    // todo consider accelerating with matrix maths (neon) if perf improvements required
    output->x = 0.5f * (
            ( 2*p[1].x ) +
            (  -p[0].x   +   p[2].x ) * t +
            ( 2*p[0].x   - 5*p[1].x   + 4*p[2].x - p[3].x) * t2 +
            (  -p[0].x   + 3*p[1].x   - 3*p[2].x + p[3].x) * t3 );

    output->y = 0.5f * (
            ( 2*p[1].y ) +
            (  -p[0].y   +   p[2].y ) * t +
            ( 2*p[0].y   - 5*p[1].y   + 4*p[2].y - p[3].y) * t2 +
            (  -p[0].y   + 3*p[1].y   - 3*p[2].y + p[3].y) * t3 );

    output->z = 0.5f * (
            ( 2*p[1].z ) +
            (  -p[0].z   +   p[2].z ) * t +
            ( 2*p[0].z   - 5*p[1].z   + 4*p[2].z - p[3].z) * t2 +
            (  -p[0].z   + 3*p[1].z   - 3*p[2].z + p[3].z) * t3 );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2] are the start, control and end points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p0 and p2
// the output pointer is the interpolated position on the curve between p0 and p2

PUBLIC KinematicsSolution_t
cartesian_point_on_quadratic_bezier( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if(points < 3)
    {
        // need 3 points for quadratic solution
        return SOLUTION_ERROR;
    }

    // start and end of bezier
    if(pos_weight <= 0.0f + FLT_EPSILON)
    {
        memcpy( output, &p[0], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    if(pos_weight >= 1.0f - FLT_EPSILON)
    {
        memcpy( output, &p[2], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    // General form for a quadratic bezier curve

    // B(t) = ((1-t)^2 * p0) + (2(1 - t) * t * p1) + (t^2 * p2) where 0 < t < 1

    //cache oft-used values to improve read-ability
    float t = pos_weight;
    float tsq = t * t;

    float omt = 1 - t;
    float omt2 = omt * omt;

    output->x = ( omt2 * p[0].x ) + ( 2 * omt * t * p[1].x ) + ( tsq * p[2].x );
    output->y = ( omt2 * p[0].y ) + ( 2 * omt * t * p[1].y ) + ( tsq * p[2].y );
    output->z = ( omt2 * p[0].z ) + ( 2 * omt * t * p[1].z ) + ( tsq * p[2].z );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1], p[2], p[3] are the start, control, control and end points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p0 and p2
// the output pointer is the interpolated position on the curve between p0 and p2

PUBLIC KinematicsSolution_t
cartesian_point_on_cubic_bezier( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if(points < 4)
    {
        // need 3 points for quadratic solution
        return SOLUTION_ERROR;
    }

    // start and end of bezier
    if(pos_weight <= 0.0f + FLT_EPSILON)
    {
        memcpy( output, &p[0], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    if(pos_weight >= 1.0f - FLT_EPSILON)
    {
        memcpy( output, &p[3], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    // General form for a cubic bezier curve

    // B(t) = ((1-t)^3 * p0) + (3(1 - t)^2 * t * P1) + (3(1-t)t^2 * P2) + (t^3 * P3) where 0 < t < 1

    //cache oft-used values to improve read-ability
    float t = pos_weight;
    float tsq = t * t;
    float tcu = tsq * t;

    float omt = 1 - t;
    float omt2 = omt * omt;
    float omt3 = omt2 * omt;

    output->x = ( omt3 * p[0].x ) + ( 3 * omt2 * t * p[1].x ) + ( 3 * omt * tsq * p[2].x ) + ( tcu * p[2].x );
    output->y = ( omt3 * p[0].y ) + ( 3 * omt2 * t * p[1].y ) + ( 3 * omt * tsq * p[2].y ) + ( tcu * p[3].y );
    output->z = ( omt3 * p[0].z ) + ( 3 * omt2 * t * p[1].z ) + ( 3 * omt * tsq * p[2].z ) + ( tcu * p[3].z );

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// p[0], p[1] are the start and end points in 3D space
// rel_weight is the 0.0-1.0 percentage position on the curve between p0 and p1
// the output pointer is the interpolated position on the curve between p0 and p1
// https://blender.stackexchange.com/questions/42131/modelling-a-spiral-around-a-sphere/42159

PUBLIC KinematicsSolution_t
cartesian_point_on_spiral( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output )
{
    if(points < 1)
    {
        // need 2 points for quadratic solution
        return SOLUTION_ERROR;
    }

    // start and end of the helix
    if(pos_weight <= 0.0f + FLT_EPSILON)
    {
        memcpy( output, &p[0], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    if(pos_weight >= 1.0f - FLT_EPSILON)
    {
        memcpy( output, &p[1], sizeof(CartesianPoint_t) );
        return SOLUTION_VALID;
    }

    // General form for a spherical spiral loxodrome (parametric equations)

    //  x(t) = cos(t) / sqrt( 1 + a^2 * t^2 )
    //	y(t) = sin(t) / sqrt( 1 + a^2 * t^2 )
    //	z(t) = - a*t  / sqrt( 1 + a^2 * t^2 )
    //  where 0 < t < 1, r is the winding radius

    uint8_t numSpirals = 7;

    //cache oft-used values to improve read-ability
    float t = pos_weight;
    float a = 1 / numSpirals;
    float denominator = sqrt(1 + a*a * t*t);

    output->x = cos(t) / denominator;
    output->y = sin(t) / denominator;
    output->z = (-1 * a * t) / denominator;

    return SOLUTION_VALID;
}

/* ----- End ---------------------------------------------------------------- */
