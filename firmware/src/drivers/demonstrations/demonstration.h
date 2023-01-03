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
demonstration_init( uint8_t selected_demo );

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
demonstration_enqueue_moves( uint8_t max_moves_emitted );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* DEMONSTRATION_H */
