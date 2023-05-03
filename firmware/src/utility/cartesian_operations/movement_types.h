#ifndef MOVEMENT_TYPES_H
#define MOVEMENT_TYPES_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "cartesian_types.h"

/* -------------------------------------------------------------------------- */

#define MOVEMENT_POINTS_COUNT 4

/* -------------------------------------------------------------------------- */

typedef enum
{
    SOLUTION_VALID,
    SOLUTION_ERROR,
} MotionSolution_t;

typedef enum
{
    _POINT_TRANSIT = 0,
    _LINE,
    _CATMULL_SPLINE,
    _CATMULL_SPLINE_LINEARISED,
    _BEZIER_QUADRATIC,
    _BEZIER_QUADRATIC_LINEARISED,
    _BEZIER_CUBIC,
    _BEZIER_CUBIC_LINEARISED,
} MotionAdjective_t;

typedef enum
{
    _POS_ABSOLUTE = 0,
    _POS_RELATIVE,
} MotionReference_t;

/* -------------------------------------------------------------------------- */

// Enums to help make array indices for motion types easier to read
typedef enum
{
    _LINE_START = 0,
    _LINE_END,
} LinePointNames_t;

typedef enum
{
    _CATMULL_CONTROL_A = 0,
    _CATMULL_START,
    _CATMULL_END,
    _CATMULL_CONTROL_B,
} CatmullPointNames_t;

typedef enum
{
    _QUADRATIC_START = 0,
    _QUADRATIC_CONTROL,
    _QUADRATIC_END,
} QuadraticPointNames_t;

typedef enum
{
    _CUBIC_START = 0,
    _CUBIC_CONTROL_A,
    _CUBIC_CONTROL_B,
    _CUBIC_END,
} CubicPointNames_t;

/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */

#endif    // MOVEMENT_TYPES_H
