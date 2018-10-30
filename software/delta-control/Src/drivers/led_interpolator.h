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

PUBLIC float
led_interpolator_get_progress( void );

/* -------------------------------------------------------------------------- */

PUBLIC bool
led_interpolator_get_move_done( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_interpolator_set_home( void );

/* -------------------------------------------------------------------------- */

#endif /* LED_INTERPOLATOR_H */
