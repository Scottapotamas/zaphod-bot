#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "lighting_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC void led_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void led_request_target( HSIColour_t *colour );

/* -------------------------------------------------------------------------- */

PUBLIC void led_set_calibration_factors( uint16_t red, uint16_t green, uint16_t blue );

PUBLIC void led_set_calibration_factors_rgb( RGBColour_t *offsets );

/* -------------------------------------------------------------------------- */

PUBLIC void led_task( void* arg );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* LED_H */