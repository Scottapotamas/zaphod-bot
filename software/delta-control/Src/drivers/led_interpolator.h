#ifndef LED_INTERPOLATOR_H
#define LED_INTERPOLATOR_H

/* ----- Local Includes ----------------------------------------------------- */

#include <led_types.h>
#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Types ------------------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
led_interpolator_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_process( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_objective( Fade_t * fade_to_process );

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_is_ready_for_next( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_start( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_stop( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_manual_override_on( void );

PUBLIC void
led_interpolator_manual_override_release( void );

PUBLIC void
led_interpolator_manual_control_set( float hue, float saturation, float intensity, bool enabled );

/* -------------------------------------------------------------------------- */

PUBLIC float
led_interpolator_get_progress( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_get_fade_done(void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_dark(void );

/* -------------------------------------------------------------------------- */

#endif /* LED_INTERPOLATOR_H */
