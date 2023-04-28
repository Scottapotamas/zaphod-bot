#ifndef MOVEMENT_TYPES_H
#define MOVEMENT_TYPES_H

#include "global.h"
#include "cartesian_types.h"

#define MOVEMENT_POINTS_COUNT 4

typedef struct
{
    unsigned id : 9;
    unsigned ref : 1;           // MotionReference_t - relative or absolute positioning frame
    unsigned type: 3;           // MotionAdjective_t - style of motion interpolation/path
    unsigned num_pts : 3;       // number of used elements in points array
} __attribute__((__packed__)) MovementBitfield_t;

typedef struct Movement__
{
    MovementBitfield_t metadata;
    uint16_t           duration;       // execution duration in milliseconds
    uint32_t           sync_offset;    // milliseconds after sync timestamp when move should execute
                                       //     used as a (timebased) global identifier
    CartesianPoint_t   points[MOVEMENT_POINTS_COUNT];    // array of 3d points
} Movement_t;

#endif    // MOVEMENT_TYPES_H
