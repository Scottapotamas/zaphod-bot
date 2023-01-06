#ifndef RANDOM_POINTS_H
#define RANDOM_POINTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "motion_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
random_points_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
random_points_get_move( uint8_t sequence_index, Movement_t *move );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* RANDOM_POINTS_H */
