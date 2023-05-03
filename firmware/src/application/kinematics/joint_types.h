#ifndef JOINT_TYPES_H
#define JOINT_TYPES_H

#include "global.h"

// Servo motor angle structure, measured in degrees
typedef struct
{
    float a1;
    float a2;
    float a3;
} JointAngles_t;

#endif    // JOINT_TYPES_H
