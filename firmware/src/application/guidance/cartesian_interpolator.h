#ifndef CARTESIAN_INTERPOLATOR_H
#define CARTESIAN_INTERPOLATOR_H

#include "global.h"
#include "cartesian_types.h"
#include "movement_types.h"

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

#endif    // CARTESIAN_INTERPOLATOR_H
