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
    unsigned num_pts: 2;            // number of used elements in points array
    unsigned type : 4;              // FadeAdjective_t for execution mode
    unsigned speed_compensated : 1; // Apply effector speed luma reduction
    unsigned positional_noise : 1;  // Apply spatial noise effect
    unsigned reserved : 7;
} __attribute__((__packed__)) FadeBitfield_t;

typedef struct
{
    uint32_t        sync_offset;                             // unique identifier of animation
    uint16_t        duration;                                // execution time in milliseconds
    FadeBitfield_t  settings;                                // Settings values
    HSIColour_t     input_colours[COLOUR_SETPOINT_COUNT];    // array of colours
} Fade_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
hsi_to_rgb( float h, float s, float i, float *r, float *g, float *b );

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */

#endif /* LED_TYPES_H */
