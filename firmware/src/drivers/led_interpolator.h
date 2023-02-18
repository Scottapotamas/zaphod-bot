#ifndef LED_INTERPOLATOR_H
#define LED_INTERPOLATOR_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "timer_ms.h"
#include <led_types.h>

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
led_interpolator_set_epoch_reference( timer_ms_t timestamp_ms );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_objective( Fade_t *fade_to_process );

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_is_ready_for_next( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_is_empty( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_start( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_stop( void );

/* -------------------------------------------------------------------------- */

PUBLIC float
led_interpolator_get_progress( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_get_fade_done( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_dark( void );

/* -------------------------------------------------------------------------- */

#endif /* LED_INTERPOLATOR_H */
