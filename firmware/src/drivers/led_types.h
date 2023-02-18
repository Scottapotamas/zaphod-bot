#ifndef LED_TYPES_H
#define LED_TYPES_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

// LED Channel power. 0 for off, 0xFFFFFFFFFFFF for full output
typedef struct
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} RGBColour_t;

typedef struct
{
    float hue;
    float saturation;
    float intensity;
} HSIColour_t;

typedef struct
{
    float x;
    float y;
    float z;
} GenericColour_t;

typedef enum
{
    FADE_VALID,
    FADE_ERROR,
} FadeSolution_t;

typedef enum
{
    _INSTANT_CHANGE = 0,
    _LINEAR_RAMP,
} FadeAdjective_t;

#define COLOUR_SETPOINT_COUNT 2

typedef struct
{
    uint32_t        sync_offset;                             // unique identifier of animation
    uint16_t        duration;                                // execution time in milliseconds
    FadeAdjective_t type;                                    // type of interpolation used between points
    uint8_t         num_pts;                                 // number of used elements in points array
    HSIColour_t     input_colours[COLOUR_SETPOINT_COUNT];    // array of colours
} Fade_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
hsi_to_rgb( float h, float s, float i, float *r, float *g, float *b );

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */

#endif /* LED_TYPES_H */
