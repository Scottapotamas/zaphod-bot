#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
led_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_enable( bool enable );

/* -------------------------------------------------------------------------- */

PUBLIC void
led_set( float r, float g, float b );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* LED_H */
