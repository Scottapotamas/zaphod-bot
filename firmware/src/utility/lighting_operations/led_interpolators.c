/* -------------------------------------------------------------------------- */

#include <float.h>
#include <string.h>

#include "led_interpolators.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

PUBLIC FadeSolution_t hsi_lerp_linear( HSIColour_t p[],
                                       size_t points,
                                       float pos_weight,
                                       HSIColour_t *output )
{
    REQUIRE( p );
    REQUIRE( points == 2);  // need 2 points for a line

    if( pos_weight <= 0.0f + FLT_EPSILON )
    {
        memcpy( output, &p[0], sizeof( HSIColour_t ) );
        return FADE_VALID;
    }

    if( pos_weight >= 1.0f - FLT_EPSILON )
    {
        memcpy( output, &p[1], sizeof( HSIColour_t ) );
        return FADE_VALID;
    }

    // Linear interpolation between two points (lerp)

    // for hue, remember it covers a circular range: we need to take the shortest path across the wrapping point
    float distance_ccw = ( p[0].hue >= p[1].hue ) ? p[0].hue - p[1].hue : 1 + p[0].hue - p[1].hue;
    float distance_cw  = ( p[0].hue >= p[1].hue ) ? 1 + p[1].hue - p[0].hue : p[1].hue - p[0].hue;

    output->hue = ( distance_cw <= distance_ccw ) ? p[1].hue + ( distance_cw * pos_weight ) : p[1].hue - ( distance_ccw * pos_weight );

    // handle wrapping around
    if( output->hue < 0.0f )
    {
        output->hue += 1.0f;
    }

    if( output->hue > 1.0f )
    {
        output->hue -= 1.0f;
    }

    output->saturation = p[0].saturation + pos_weight * ( p[1].saturation - p[0].saturation );
    output->intensity  = p[0].intensity + pos_weight * ( p[1].intensity - p[0].intensity );

    return FADE_VALID;
}

/* -------------------------------------------------------------------------- */