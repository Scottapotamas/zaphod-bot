/* ----- System Includes ---------------------------------------------------- */
#define _USE_MATH_DEFINES
#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"
#include "kinematics.h"

#include "app_times.h"
#include "configuration.h"
#include "motion_types.h"
#include "user_interface.h"

/* ----- Defines ------------------------------------------------------------ */

// position offset between kinematics space and cartesian user-space
CartesianPoint_t offset_position = {
    .x = 0,
    .y = 0,
    .z = MM_TO_MICRONS( KINEMATICS_Z_OFFSET_MM )
};

// Constrain motion to the practical parts of the movement volume
int32_t z_max = MM_TO_MICRONS( KINEMATICS_Z_MAX_MM );
int32_t z_min = MM_TO_MICRONS( KINEMATICS_Z_MIN_MM );

int32_t radius = MM_TO_MICRONS( KINEMATICS_RADIUS_MAX_MM );

// Rotate the cartesian co-ordinate space
int8_t flip_x = 1;
int8_t flip_y = 1;
int8_t flip_z = -1;

// Delta geometry defines
float f  = MM_TO_MICRONS( 50.0f );     // radius of motor shafts on base
float rf = MM_TO_MICRONS( 180.0f );    // base joint to elbow joint distance
float re = MM_TO_MICRONS( 340.0f );    // elbow joint to end affector joint
float e  = MM_TO_MICRONS( 34.0f );     // end effector joint radius

// cache common trig constants
float sqrt3;
float sqrt1div3;
float sin120;
float sin30;
float cos120;
float tan60;
float tan30;

float deg_to_rad;

// Cache common calculations
float t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE KinematicsSolution_t
delta_angle_plane_calc( float x0, float y0, float z0, float *theta );

PRIVATE void
kinematics_clamp_volume( CartesianPoint_t *point );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
kinematics_init( void )
{
    // calculate/cache common trig constants
    sqrt3     = M_SQRT3;
    sqrt1div3 = sqrtf( 1.0f / 3.0f );
    sin120    = sqrt3 / 2.0f;
    sin30     = 0.5f;
    cos120    = -0.5f;
    tan60     = sqrt3;
    tan30     = 1 / sqrt3;

    // cache common calculations
    deg_to_rad = M_PI / 180.0f;
    t          = ( f - e ) * tan30 / 2;

    user_interface_set_kinematics_mechanism_info( f, rf, re, e );
    user_interface_set_kinematics_limits( radius, z_min, z_max );
    user_interface_set_kinematics_flips( flip_x, flip_y, flip_z );
}

/* -------------------------------------------------------------------------- */

/*
 * Clamps the position within the allowable cylindrical shaped workspace
 *
 * We assume (because duh) that the cylinder is centered at x = 0, y = 0,
 * pointing 'up' -> height following the vertical axis (z).
 *
 * The check+clamp is performed as a chain of 'most common' conditions to exit early.
 * - if the point is within a naiive diamond area contained within the circle, exit
 * - perform a pythagorean distance check against the radius
 * - Calculate a scalar to clamp illegal positions to the circle radius
 */

PRIVATE void
kinematics_clamp_volume( CartesianPoint_t *point )
{
    // Check 'height' is within the bounds
    // TODO: check for user-configured height
    point->z = CLAMP( point->z, z_min, z_max );

    uint32_t dx = abs( point->x );    // if using off-center circle, use abs(x - center_x)
    uint32_t dy = abs( point->y );

    // Quickly check if the point is outside the radius-sized diamond area inside the circle
    // TODO: check for user-configured radius limit
    if( ( dx + dy ) >= radius )
    {
        // Calculate and compare the distance properly
        float distance2 = (float)dx * (float)dx + (float)dy * (float)dy;
        float radius2   = (float)radius * (float)radius;

        // Clamp position within the circle's radius
        if( distance2 >= radius2 )    // Pythagorean distance check
        {
            float scale_factor = radius2 / distance2;
            point->x *= scale_factor;
            point->y *= scale_factor;
        }
    }

    // TODO consider conical volume restrictions to maximise usable area
}

/* -------------------------------------------------------------------------- */

/*
 * Accept an x/y/z cartesian input, write into provided pointer to angle structure
 * Returns 0 when OK, 1 for error
 *
 * Calculate the output motor angles with an IK solver
 * Bounds checks to ensure motors aren't being commanded past their practical limits
 * Set the target angles for the Clearpath driver to then handle.
 *
 * Returns status
 */

PUBLIC KinematicsSolution_t
kinematics_point_to_angle( CartesianPoint_t input, JointAngles_t *output )
{
    // Limit attempts at out-of-bounds positions
    kinematics_clamp_volume( &input );

    // Offset the work-area position frame into the kinematics domain position
    input.x = ( input.x + offset_position.x ) * flip_x;
    input.y = ( input.y + offset_position.y ) * flip_y;
    input.z = ( input.z + offset_position.z ) * flip_z;

    // Perform kinematics calculations
    KinematicsSolution_t status = delta_angle_plane_calc( input.x, input.y, input.z, &output->a1 );

    if( status == KINEMATICS_SOLVE_OK )
    {
        // Rotate +120 degrees
        status = delta_angle_plane_calc( input.x * cos120 + input.y * sin120,
                                         input.y * cos120 - input.x * sin120,
                                         input.z,
                                         &output->a2 );
    }

    if( status == KINEMATICS_SOLVE_OK )
    {
        // Rotate -120 degrees
        status = delta_angle_plane_calc( input.x * cos120 - input.y * sin120,
                                         input.y * cos120 + input.x * sin120,
                                         input.z,
                                         &output->a3 );
    }

    return status;
}

/* -------------------------------------------------------------------------- */

/*
 * Accept angle 1,2,3 input, write into provided pointer to cartesian point structure
 * Returns 0 when OK, 1 for error
 *
 * Calculate the cartesian co-ordinates with the FK solver
 * Emit the XYZ co-ordinates
 */

PUBLIC KinematicsSolution_t
kinematics_angle_to_point( JointAngles_t input, CartesianPoint_t *output )
{
    input.a1 *= deg_to_rad;
    input.a2 *= deg_to_rad;
    input.a3 *= deg_to_rad;

    float y1 = -( t + rf * cosf( input.a1 ) );
    float z1 = -rf * sinf( input.a1 );

    float y2 = ( t + rf * cosf( input.a2 ) ) * sin30;
    float x2 = y2 * tan60;
    float z2 = -rf * sinf( input.a2 );

    float y3 = ( t + rf * cosf( input.a3 ) ) * sin30;
    float x3 = -y3 * tan60;
    float z3 = -rf * sinf( input.a3 );

    float dnm = ( y2 - y1 ) * x3 - ( y3 - y1 ) * x2;

    float w1 = y1 * y1 + z1 * z1;
    float w2 = x2 * x2 + y2 * y2 + z2 * z2;
    float w3 = x3 * x3 + y3 * y3 + z3 * z3;

    // x = (a1*z + b1)/dnm
    float a1 = ( z2 - z1 ) * ( y3 - y1 ) - ( z3 - z1 ) * ( y2 - y1 );
    float b1 = -( ( w2 - w1 ) * ( y3 - y1 ) - ( w3 - w1 ) * ( y2 - y1 ) ) / 2.0f;

    // y = (a2*z + b2)/dnm;
    float a2 = -( z2 - z1 ) * x3 + ( z3 - z1 ) * x2;
    float b2 = ( ( w2 - w1 ) * x3 - ( w3 - w1 ) * x2 ) / 2.0f;

    // a*z^2 + b*z + c = 0
    float a = a1 * a1 + a2 * a2 + dnm * dnm;
    float b = 2 * ( a1 * b1 + a2 * ( b2 - y1 * dnm ) - z1 * dnm * dnm );
    float c = ( b2 - y1 * dnm ) * ( b2 - y1 * dnm ) + b1 * b1 + dnm * dnm * ( z1 * z1 - re * re );

    // discriminant
    float d = b * b - (float)4.0f * a * c;

    if( d < 0.0f )
    {
        return SOLUTION_ERROR;
    }

    output->z = -(float)0.5f * ( b + sqrtf( d ) ) / a;
    output->x = ( a1 * output->z + b1 ) / dnm;
    output->y = ( a2 * output->z + b2 ) / dnm;

    //todo correct the FK returned co-ordinates to undo the translations made in the IK stage

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// helper functions, calculates angle theta1 (for YZ-pane)
PRIVATE KinematicsSolution_t
delta_angle_plane_calc( float x0, float y0, float z0, float *theta )
{
    float y1 = -0.5f * sqrt1div3 * f;    // f/2 * tg 30
    y0 -= 0.5f * sqrt1div3 * e;          // Shift center to edge

    // z = a + b*y
    float a = ( x0 * x0 + y0 * y0 + z0 * z0 + rf * rf - re * re - y1 * y1 ) / ( 2.0f * z0 );
    float b = ( y1 - y0 ) / z0;

    // Discriminant
    float d = -( a + b * y1 ) * ( a + b * y1 ) + rf * ( b * b * rf + rf );

    if( d < 0.0f )
    {
        return KINEMATICS_SOLVE_ERROR;
    }

    float yj = ( y1 - a * b - sqrtf( d ) ) / ( b * b + 1 );    // choose the outer point
    float zj = a + b * yj;

    *theta = 180.0f * atanf( -zj / ( y1 - yj ) ) / M_PI + ( ( yj > y1 ) ? 180.0f : 0.0f );

    return KINEMATICS_SOLVE_OK;
}

/* ----- End ---------------------------------------------------------------- */
