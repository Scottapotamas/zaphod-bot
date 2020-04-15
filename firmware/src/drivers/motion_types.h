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
    MotionAdjective_t type;                             // style of motion interpolation/path
    MotionReference_t ref;                              // relative or absolute positioning frame
    uint16_t          identifier;                       // unique identifier of movement
    uint16_t          duration;                         // execution time in milliseconds
    uint16_t          num_pts;                          // number of used elements in points array
    CartesianPoint_t  points[MOVEMENT_POINTS_COUNT];    // array of 3d points
} Movement_t;

typedef uint32_t mm_per_second_t;
typedef uint32_t micron_per_millisecond_t;

/* ----- Functions ---------------------------------------------------------- */

PUBLIC mm_per_second_t
cartesian_move_speed( Movement_t *movement );

PUBLIC int32_t
cartesian_move_distance( Movement_t *movement );

PUBLIC void
cartesian_point_rotate_around_z( CartesianPoint_t *a, float degrees );

PUBLIC int32_t
cartesian_distance_between( CartesianPoint_t *a, CartesianPoint_t *b );

PUBLIC int16_t
cartesian_duration_for_speed( CartesianPoint_t *a, CartesianPoint_t *b, mm_per_second_t target_speed );

PUBLIC KinematicsSolution_t
cartesian_point_on_line( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

PUBLIC KinematicsSolution_t
cartesian_point_on_catmull_spline( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

PUBLIC KinematicsSolution_t
cartesian_point_on_quadratic_bezier( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

PUBLIC KinematicsSolution_t
cartesian_point_on_cubic_bezier( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

PUBLIC KinematicsSolution_t
cartesian_point_on_spiral( CartesianPoint_t *p, size_t points, float pos_weight, CartesianPoint_t *output );

/* -------------------------------------------------------------------------- */

#endif /* MOTION_TYPES_H */
