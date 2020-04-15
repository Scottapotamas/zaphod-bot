#ifndef LED_TYPES_H
#define LED_TYPES_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

// LED Channel power. 0 for off, 0xFFFFFF for full output
typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
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
    uint16_t        identifier;    // unique identifier of animation
    uint16_t        duration;      // execution time in milliseconds
    FadeAdjective_t type;          // type of interpolation used between points
    uint8_t         num_pts;       // number of used elements in points array
    //padding x2
    HSIColour_t input_colours[COLOUR_SETPOINT_COUNT];    //array of colours
} Fade_t;

/* -------------------------------------------------------------------------- */

#endif /* LED_TYPES_H */
