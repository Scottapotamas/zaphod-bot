#ifndef MOTION_TYPES_H
#define MOTION_TYPES_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

// Co-ordinate space in xyz, measured in microns.
// This provides micron resolution positioning to 2147 meters...
// for reference, 16-bit positioning would bound the axis to 65mm travel
typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} CartesianPoint_t;

typedef struct
{
    float a1;
    float a2;
    float a3;
} JointAngles_t;

typedef enum
{
	SOLUTION_VALID,
	SOLUTION_ERROR,
} KinematicsSolution_t;


typedef enum
{
	_POINT_TRANSIT = 0,
	_LINE,
	_CATMULL_SPLINE,
	_BEZIER_QUADRATIC,
	_BEZIER_CUBIC,
} MotionAdjective_t;

typedef enum
{
	_POS_ABSOLUTE = 0,
	_POS_RELATIVE,
} MotionReference_t;

#define MOVEMENT_POINTS_COUNT 4

typedef struct
{
	MotionAdjective_t type;	// MotionAdjective_t
	MotionReference_t ref;	// MotionReference_t
    uint16_t duration;		// execution time in milliseconds
    uint8_t identifier;		//unique identifier of movement
    uint16_t num_pts;			//number of used elements in points array
    //padding
    CartesianPoint_t points[ MOVEMENT_POINTS_COUNT + 1 ];	//array of 3d points
} Movement_t;

/* -------------------------------------------------------------------------- */

#endif /* MOTION_TYPES_H */
