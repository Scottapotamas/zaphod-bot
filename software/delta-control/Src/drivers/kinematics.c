/* ----- System Includes ---------------------------------------------------- */

#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "kinematics.h"
#include "clearpath.h"

#include "global.h"
#include "qassert.h"

/* ----- Defines ------------------------------------------------------------ */


//delta geometry defines
float f  = 50.0;    	// radius of motor shafts on base
float rf = 180.0;    	// base joint to elbow joint distance
float re = 330.0;    	// elbow joint to end affector joint
float e  = 32.0;    	// end effector joint radius

// cache common trig constants
float sqrt3;
float sin120;
float sin30;
float cos120;
float tan60;
float tan30;
float dtr;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE uint16_t
delta_angle_plane_calc( float x0, float y0, float z0, float *theta );


/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
kinematics_init( )
{
	// calculate/cache common trig constants
	sqrt3  = sqrt( 3.0f );
	sin120 = sqrt3 / 2.0f;
	sin30  = 0.5f;
	cos120 = -0.5f;
	tan60  = sqrt3;
	tan30  = 1 / sqrt3;
    dtr = M_PI / 180.0f;

}

/* -------------------------------------------------------------------------- */

/*
 * Accept a x/y/z input from the motion planner
 *
 * Calculate the output motor angles with an IK solver
 * Bounds checks to ensure motors aren't being commanded past their practical limits
 * Set the target angles for the clearpath driver to then handle.
 */

PUBLIC uint8_t
kinematics_point_to_angle( void )
{
	// inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
	// returned status: 0 = OK, -1 = invalid position
	uint32_t x0 = 0;
	uint32_t y0 = 0;
	uint32_t z0 = 0;

    float theta1 = 0;
    float theta2 = 0;
    float theta3 = 0;

    uint8_t status = delta_angle_plane_calc( x0, y0, z0, &theta1 );

    if (status == 0)
    {
    	// rotate +120 degrees
    	status = delta_angle_plane_calc( 	x0*cos120 + y0*sin120,
    										y0*cos120 - x0*sin120,
											z0,
											&theta2   );
    }

    if (status == 0)
    {
    	// rotate -120 degrees
    	status = delta_angle_plane_calc( 	x0*cos120 - y0*sin120,
    										y0*cos120 + x0*sin120,
											z0,
											&theta3 );
    }

     return status;
}

/* -------------------------------------------------------------------------- */

/*
 * Accept a a/b/c angle input
 *
 * Calculate the cartesian co-ordinates with the FK solver
 * Emit the XYZ co-ordinates
 */

PUBLIC uint8_t
kinematics_angle_to_point( void )
{
	float theta1 = 0;
	float theta2 = 0;
	float theta3 = 0;

	uint32_t x0 = 0;
	uint32_t y0 = 0;
	uint32_t z0 = 0;

    float t = ( f-e ) * tan30/2;

    theta1 *= dtr;
    theta2 *= dtr;
    theta3 *= dtr;

    float y1 = -( t + rf*cos(theta1) );
    float z1 = -rf * sin(theta1);

    float y2 = ( t + rf*cos(theta2) ) * sin30;
    float x2 = y2 * tan60;
    float z2 = -rf * sin(theta2);

    float y3 = (t + rf*cos(theta3)) * sin30;
    float x3 = -y3 * tan60;
    float z3 = -rf * sin(theta3);

    float dnm = (y2-y1)*x3-(y3-y1)*x2;

    float w1 = y1*y1 + z1*z1;
    float w2 = x2*x2 + y2*y2 + z2*z2;
    float w3 = x3*x3 + y3*y3 + z3*z3;

    // x = (a1*z + b1)/dnm
    float a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);
    float b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0;

    // y = (a2*z + b2)/dnm;
    float a2 = -(z2-z1)*x3+(z3-z1)*x2;
    float b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0;

    // a*z^2 + b*z + c = 0
    float a = a1*a1 + a2*a2 + dnm*dnm;
    float b = 2*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
    float c = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - re*re);

    // discriminant
    float d = b*b - (float)4.0*a*c;

    if (d < 0)
    {
    	return -1; // impossible point
    }

    z0 = -(float)0.5*(b+sqrt(d))/a;
    x0 = (a1*z0 + b1) / dnm;
    y0 = (a2*z0 + b2) / dnm;

    return 0;
}

/* -------------------------------------------------------------------------- */

// helper functions, calculates angle theta1 (for YZ-pane)
PRIVATE uint16_t
delta_angle_plane_calc(float x0, float y0, float z0, float *theta)
{
    float y1 = -0.5 * 0.57735 * f; 		// f/2 * tg 30
    y0 		 -= 0.5 * 0.57735 * e;      // shift center to edge

    // z = a + b*y
    float a = ( x0*x0 + y0*y0 + z0*z0 + rf*rf - re*re - y1*y1 ) / ( 2 * z0 );
    float b = ( y1 - y0 ) / z0;

    // discriminant
    float d = -( a+b*y1 )*( a+b * y1 ) + rf*( b*b * rf+rf );

    if (d < 0)
    {
    	return -1; // impossible point
    }

    float yj = ( y1 - a*b - sqrt(d) ) / ( b*b + 1 ); // choose the outer point
    float zj = a + b*yj;

    *theta = 180.0 * atan( -zj/(y1 - yj) ) / M_PI + ( (yj > y1) ? 180.0 : 0.0 );

    return 0;
}

/* ----- End ---------------------------------------------------------------- */
