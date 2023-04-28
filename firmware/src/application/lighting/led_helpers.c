/* -------------------------------------------------------------------------- */

#include "led_helpers.h"
#include "qassert.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

PRIVATE float
hue_to_channel( float p, float q, float t );

/* -------------------------------------------------------------------------- */

/* Converts a Hue/Saturation/Intensity colour to a Red/Green/Blue one
 * Input HSI are [0, 1]
 * Output RGB are [0, 1]
 */
PUBLIC void
hsi_to_rgb( float h, float s, float i, float *r, float *g, float *b )
{
    REQUIRE( r );
    REQUIRE( g );
    REQUIRE( b );

    float q;
    float p;

    // no saturation --> white/achromatic at the given intensity level
    if( s <= 0.0f )
    {
        *r = i;
        *g = i;
        *b = i;
    }
    else
    {
        if( i < 0.5 )
        {
            q = i * ( 1.0f + s );
        }
        else
        {
            q = i + s - i * s;
        }

        p = 2 * i - q;

        *r = hue_to_channel( p, q, h + 1.0f / 3.0f );
        *g = hue_to_channel( p, q, h );
        *b = hue_to_channel( p, q, h - 1.0f / 3.0f );
    }
}

/* -------------------------------------------------------------------------- */

// Helper for HSI/RGB conversions
PRIVATE float
hue_to_channel( float p, float q, float t )
{
    if( t < 0.0f )
    {
        t += 1.0f;
    }

    if( t > 1.0f )
    {
        t -= 1.0f;
    }

    if( t < 1.0f / 6.0f )
    {
        return p + ( q - p ) * 6.0f * t;
    }

    if( t < 1.0f / 2.0f )
    {
        return q;
    }

    if( t < 2.0f / 3.0f )
    {
        return p + ( q - p ) * ( 2.0f / 3.0f - t ) * 6.0f;
    }

    return p;
}

/* -------------------------------------------------------------------------- */