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
shutter_capture( bool on );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* SHUTTER_RELEASE_H */
