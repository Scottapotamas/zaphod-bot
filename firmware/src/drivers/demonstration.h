#ifndef DEMONSTRATION_H
#define DEMONSTRATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
demonstration_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
demonstration_prepare_demo_move( uint8_t demo_index );

/* -------------------------------------------------------------------------- */

PUBLIC void
demonstration_prepare_sequence( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* DEMONSTRATION_H */
