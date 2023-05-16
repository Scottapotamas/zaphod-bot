#ifndef INTERNAL_SEQUENCE_H
#define INTERNAL_SEQUENCE_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "motion_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
internal_sequence_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
internal_sequence_get_move( uint8_t sequence_index, Movement_t *move );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* INTERNAL_SEQUENCE_H */
