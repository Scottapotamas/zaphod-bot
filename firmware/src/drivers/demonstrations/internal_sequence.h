#ifndef INTERNAL_SEQUENCE_H
#define INTERNAL_SEQUENCE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
internal_sequence_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
internal_sequence_prepare_demo_move( uint8_t demo_index );

/* -------------------------------------------------------------------------- */

PUBLIC void
internal_sequence_prepare_sequence( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* INTERNAL_SEQUENCE_H */
