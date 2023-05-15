/* -------------------------------------------------------------------------- */

#include <math.h>

#include "noise.h"

/* -------------------------------------------------------------------------- */

PRIVATE float fbm( float value );

/* -------------------------------------------------------------------------- */

// 1D Noise function ported from https://www.shadertoy.com/view/3sd3Rs
// Fully deterministic, periodic and doesn't require caching/lookups like typical Perlin noise generators
PUBLIC float
bnoise( float x )
{
    float temp; // modf needs to return the integer part somewhere

    // Setup
    float i = floorf( x );
    float fract = modff( x, &temp );
    float sign = copysignf( 1.0f, modff(x/2.0f, &temp ) - 0.5f );

    // Create a random value k in [0..1]
    float k = modff( i * 0.1731f, &temp );

    // Quartic polynomial
    return   sign
           * fract
           * ( fract - 1.0f )
           * ( ( 16.0f * k - 4.0f ) * fract * ( fract - 1.0f ) - 1.0f );
}

// Fractal Brownian Motion
// Superimposes the noise function for a range of scales
PRIVATE float
fbm( float value )
{
    const uint8_t octaves = 3; // Higher count increases the fine detail/complexity of the noise

    float noise = 0.0f;
    float amplitude = 1.0f;

    for( uint8_t i = 0; i < octaves; i++ )
    {
        noise += amplitude * bnoise(value );

        // Next iteration should have a lower amplitude and higher frequency
        amplitude *= 0.5f;
        value *= 2.0f;  // Increase frequency
        value += 0.131f; // Offset to mitigate the noise function returning zero for integer/zero positions
    }

    return noise;
}

/* -------------------------------------------------------------------------- */