#ifndef KINEMATICS_H
#define KINEMATICS_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "cartesian_types.h"
#include "joint_types.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    KINEMATICS_SOLVE_OK,
    KINEMATICS_SOLVE_ERROR,
} KinematicsSolution_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
kinematics_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC KinematicsSolution_t
kinematics_point_to_angle( CartesianPoint_t input, JointAngles_t *output );

/* -------------------------------------------------------------------------- */

PUBLIC KinematicsSolution_t
kinematics_angle_to_point( JointAngles_t input, CartesianPoint_t *output );

/* -------------------------------------------------------------------------- */

#endif /* KINEMATICS_H */