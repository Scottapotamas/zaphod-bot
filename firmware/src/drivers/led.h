#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "led_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
led_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_enable( bool enable );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_request_dark( void );

PUBLIC void
led_request_rgb( float r, float g, float b );

PUBLIC void
led_request_hsi( HSIColour_t color );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_update_speed_luma_factor( uint32_t mm_second );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_refresh_output( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* LED_H */
