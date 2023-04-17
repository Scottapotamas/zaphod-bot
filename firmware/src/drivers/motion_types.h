#ifndef MOTION_TYPES_H
#define MOTION_TYPES_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

// Co-ordinate space in xyz, measured in microns.
// This provides micron resolution positioning to 2147 meters...
// for reference, 16-bit positioning would constrain the axis to 65 mm travel
typedef struct
{
    int32_t x;
    int32_t y;
    int32_t z;
} CartesianPoint_t;

// Servo motor angle structure, measured in degrees
typedef struct
{
    float a1;
    float a2;
    float a3;
} JointAngles_t;

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

typedef uint32_t mm_per_second_t;
typedef uint32_t micron_per_millisecond_t;

/* ----- Functions ---------------------------------------------------------- */

PUBLIC mm_per_second_t
cartesian_move_speed( Movement_t *movement );

PUBLIC uint32_t
cartesian_move_distance( Movement_t *movement );

PUBLIC void
cartesian_point_rotate_around_z( CartesianPoint_t *a, float degrees );

PUBLIC float
cartesian_distance_linearisation_from_lut( uint32_t sync_offset, float progress );

PUBLIC void
cartesian_find_point_on_line( CartesianPoint_t *a, CartesianPoint_t *b, CartesianPoint_t *p, float weight );

PUBLIC uint32_t
cartesian_distance_between( CartesianPoint_t *a, CartesianPoint_t *b );

PUBLIC uint32_t
cartesian_duration_for_speed( CartesianPoint_t *a, CartesianPoint_t *b, mm_per_second_t target_speed );

PUBLIC MotionSolution_t
cartesian_plan_smoothed_line( Movement_t *movement, float start_weight, float end_weight );

PUBLIC MotionSolution_t
cartesian_point_on_line( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

PUBLIC MotionSolution_t
cartesian_point_on_catmull_spline( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

PUBLIC MotionSolution_t
cartesian_point_on_quadratic_bezier( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

PUBLIC MotionSolution_t
cartesian_point_on_cubic_bezier( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

PUBLIC MotionSolution_t
cartesian_point_on_spiral( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

/* -------------------------------------------------------------------------- */

#endif /* MOTION_TYPES_H */
