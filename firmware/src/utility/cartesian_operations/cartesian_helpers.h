#ifndef CARTESIAN_HELPERS_H
#define CARTESIAN_HELPERS_H

#include "global.h"
#include "cartesian_types.h"
#include "movement_types.h"

#define SPEED_SAMPLE_RESOLUTION 15

PUBLIC mm_per_second_t
cartesian_move_speed( Movement_t *movement );

PUBLIC uint32_t
cartesian_move_distance( Movement_t *movement );

PUBLIC void
cartesian_apply_rotation_offset( Movement_t *move, float offset_degrees );

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


#endif    // CARTESIAN_HELPERS_H
