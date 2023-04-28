#ifndef CARTESIAN_TYPES_H
#define CARTESIAN_TYPES_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} CartesianPoint_t;

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

typedef uint32_t mm_per_second_t;
typedef uint32_t micron_per_millisecond_t;

/* ----- Functions ---------------------------------------------------------- */


/* -------------------------------------------------------------------------- */

#endif /* CARTESIAN_TYPES_H */
