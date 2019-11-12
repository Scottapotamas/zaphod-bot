#ifndef SHUTTER_RELEASE_H
#define SHUTTER_RELEASE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
shutter_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
shutter_capture( uint32_t duration_ms );

/* -------------------------------------------------------------------------- */

PUBLIC void
shutter_cancel( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
shutter_is_exposing( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
shutter_process( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* SHUTTER_RELEASE_H */
