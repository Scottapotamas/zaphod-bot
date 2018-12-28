/* ----- System Includes ---------------------------------------------------- */
#define _USE_MATH_DEFINES
#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "kinematics.h"
#include "motion_types.h"
#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

//position offset
CartesianPoint_t offset_position = {
    x: 0,
    y: 0,
    z: MM_TO_MICRONS( 190 )
};


//rotate the cartesian co-ordinate space
int8_t rotate_x = 1;
int8_t rotate_y = 1;
int8_t rotate_z = -1;

//delta geometry defines
float f  = MM_TO_MICRONS( 50.0f );    // radius of motor shafts on base
float rf = MM_TO_MICRONS( 180.0f );   // base joint to elbow joint distance
float re = MM_TO_MICRONS( 340.0f );   // elbow joint to end affector joint
float e  = MM_TO_MICRONS( 34.0f );    // end effector joint radius

// cache common trig constants
float sqrt3;
float sin120;
float sin30;
float cos120;
float tan60;
float tan30;

float deg_to_rad;

//cache common calculations
float t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE KinematicsSolution_t
delta_angle_plane_calc( float x0, float y0, float z0, float *theta );


/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
kinematics_init( void )
{
	// calculate/cache common trig constants
	sqrt3  = sqrt( 3.0f );
	sin120 = sqrt3 / 2.0f;
	sin30  = 0.5f;
	cos120 = -0.5f;
	tan60  = sqrt3;
	tan30  = 1 / sqrt3;

	//cache common calculations
    deg_to_rad = M_PI / 180.0f;
    t = ( f-e ) * tan30/2;

}

/* -------------------------------------------------------------------------- */

/*
 * Accept a x/y/z cartesian input, write into provided pointer to angle structure
 * Returns 0 when OK, 1 for error
 *
 * Calculate the output motor angles with an IK solver
 * Bounds checks to ensure motors aren't being commanded past their practical limits
 * Set the target angles for the clearpath driver to then handle.
 *
 * Returns status
 */

PUBLIC KinematicsSolution_t
kinematics_point_to_angle( CartesianPoint_t input, JointAngles_t *output )
{
	//offset the work-area position frame into the kinematics domain position
	input.x = ( input.x + offset_position.x ) * rotate_x;
	input.y = ( input.y + offset_position.y ) * rotate_y;
	input.z = ( input.z + offset_position.z ) * rotate_z;

    uint8_t status = delta_angle_plane_calc( input.x, input.y, input.z, &output->a1 );

    if (status == SOLUTION_VALID)
    {
    	// rotate +120 degrees
    	status = delta_angle_plane_calc( 	input.x*cos120 + input.y*sin120,
    										input.y*cos120 - input.x*sin120,
											input.z,
											&output->a2   );
    }

    if (status == SOLUTION_VALID)
    {
    	// rotate -120 degrees
    	status = delta_angle_plane_calc( 	input.x*cos120 - input.y*sin120,
    										input.y*cos120 + input.x*sin120,
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

    float y1 = -( t + rf*cos(input.a1) );
    float z1 = -rf * sin(input.a1);

    float y2 = ( t + rf*cos(input.a2) ) * sin30;
    float x2 = y2 * tan60;
    float z2 = -rf * sin(input.a2);

    float y3 = ( t + rf*cos(input.a3) ) * sin30;
    float x3 = -y3 * tan60;
    float z3 = -rf * sin(input.a3);

    float dnm = (y2-y1)*x3 - (y3-y1)*x2;

    float w1 = y1*y1 + z1*z1;
    float w2 = x2*x2 + y2*y2 + z2*z2;
    float w3 = x3*x3 + y3*y3 + z3*z3;

    // x = (a1*z + b1)/dnm
    float a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);
    float b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0f;

    // y = (a2*z + b2)/dnm;
    float a2 = -(z2-z1)*x3+(z3-z1)*x2;
    float b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0f;

    // a*z^2 + b*z + c = 0
    float a = a1*a1 + a2*a2 + dnm*dnm;
    float b = 2*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
    float c = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - re*re);

    // discriminant
    float d = b*b - (float)4.0f*a*c;

    if (d < 0)
    {
    	return SOLUTION_ERROR;
    }

    output->z = -(float)0.5f*(b+sqrt(d))/a;
    output->x = (a1*output->z + b1) / dnm;
    output->y = (a2*output->z + b2) / dnm;

    //todo correct the FK returned co-ordinates to undo the translations made in the IK stage

    return SOLUTION_VALID;
}

/* -------------------------------------------------------------------------- */

// helper functions, calculates angle theta1 (for YZ-pane)
PRIVATE KinematicsSolution_t
delta_angle_plane_calc(float x0, float y0, float z0, float *theta)
{
    float y1 = -0.5f * 0.57735f * f; 		// f/2 * tg 30
    y0 		 -= 0.5f * 0.57735f * e;      // shift center to edge

    // z = a + b*y
    float a = ( x0*x0 + y0*y0 + z0*z0 + rf*rf - re*re - y1*y1 ) / ( 2.0f * z0 );
    float b = ( y1 - y0 ) / z0;

    // discriminant
    float d = -( a+b * y1 )*( a+b * y1 ) + rf*( b*b * rf+rf );

    if (d < 0)
    {
    	return SOLUTION_ERROR;
    }

    float yj = ( y1 - a*b - sqrt(d) ) / ( b*b + 1 ); // choose the outer point
    float zj = a + b*yj;

    *theta = 180.0f * atan( -zj/(y1 - yj) ) / M_PI + ( (yj > y1) ? 180.0f : 0.0f );

    return SOLUTION_VALID;
}

/* ----- End ---------------------------------------------------------------- */
