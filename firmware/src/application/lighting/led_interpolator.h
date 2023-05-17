#ifndef LED_INTERPOLATOR_H
#define LED_INTERPOLATOR_H

/* -------------------------------------------------------------------------- */

#include "global.h"

#include "lighting_types.h"

/* -------------------------------------------------------------------------- */

//! The callback function which led_interpolator will call with a request HSI
typedef void (*HSIRequestFn)(HSIColour_t *position);

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_update_output_callback( HSIRequestFn callback );

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_set_epoch_reference( uint32_t sync_timer );

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t led_interpolator_queue_request( Fade_t *fade_to_process );

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_cleanup( void );

/* -------------------------------------------------------------------------- */

PUBLIC void led_interpolator_task( void* arg );

/* -------------------------------------------------------------------------- */

#endif /* LED_INTERPOLATOR_H */