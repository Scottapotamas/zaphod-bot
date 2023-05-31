#ifndef SHUTTER_RELEASE_H
#define SHUTTER_RELEASE_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "observer.h"

/* -------------------------------------------------------------------------- */

PUBLIC void shutter_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC Observer * shutter_get_observer( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* SHUTTER_RELEASE_H */