#ifndef SHUTTER_RELEASE_H
#define SHUTTER_RELEASE_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void shutter_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void shutter_task( void* arg );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* SHUTTER_RELEASE_H */