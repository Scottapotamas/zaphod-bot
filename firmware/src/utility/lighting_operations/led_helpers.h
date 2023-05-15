#ifndef LED_HELPERS_H
#define LED_HELPERS_H

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
hsi_to_rgb( float h, float s, float i, float *r, float *g, float *b );

/* -------------------------------------------------------------------------- */

PUBLIC float
led_luminance_correct( float input );

/* -------------------------------------------------------------------------- */

#endif /* LED_HELPERS_H */
