#ifndef LED_INTERPOLATORS_H
#define LED_INTERPOLATORS_H

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "lighting_types.h"

/* -------------------------------------------------------------------------- */

PUBLIC FadeSolution_t hsi_lerp_linear( HSIColour_t p[],
                                       size_t points,
                                       float pos_weight,
                                       HSIColour_t *output );

/* -------------------------------------------------------------------------- */

#endif /* LED_INTERPOLATORS_H */