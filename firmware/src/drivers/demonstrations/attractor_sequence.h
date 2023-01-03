#ifndef ATTRACTOR_SEQUENCE_H
#define ATTRACTOR_SEQUENCE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "motion_types.h"

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_sequence_init( uint8_t species );

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_sequence_get_move( uint8_t index, Movement_t *move );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* ATTRACTOR_SEQUENCE_H */
