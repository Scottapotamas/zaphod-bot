#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "lighting_types.h"
#include "observer.h"

/* -------------------------------------------------------------------------- */

PUBLIC void led_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC Observer * led_get_observer( void );

/* -------------------------------------------------------------------------- */

PUBLIC void led_request_target( HSIColour_t *colour );

/* -------------------------------------------------------------------------- */

PUBLIC void led_task( void* arg );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* LED_H */