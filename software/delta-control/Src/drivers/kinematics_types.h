#ifndef KINEMATICS_TYPES_H
#define KINEMATICS_TYPES_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

typedef struct
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
} CartesianPoint_t;

typedef struct
{
    float a1;
    float a2;
    float a3;
} JointAngles_t;

typedef enum
{
	_SOLUTION_VALID,
	_SOLUTION_ERROR,
} KinematicsSolution_t;

/* -------------------------------------------------------------------------- */

#endif /* KINEMATICS_TYPES_H */
