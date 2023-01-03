#ifndef ATTRACTOR_TYPES_H
#define ATTRACTOR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

enum {
    LORENZ = 0,
    THOMAS,
    AIZAWA,
    DADRAS,
    CHEN,
    ROSSLER,
    HALVORSEN,
    RABINOVICHFABRIKANT,
    THREESCROLL,
    SPROTT,
    FOURWING,
    MAX_VARIANTS,
} ATTRACTOR_VARIANTS;

/* -------------------------------------------------------------------------- */

enum {
    PARAM_A = 0,
    PARAM_B,
    PARAM_C,
    PARAM_D,
    PARAM_E,
    PARAM_F,
    MAX_PARAMETERS,
} ATTRACTOR_PARAMS;

/* -------------------------------------------------------------------------- */

typedef struct
{
    float x;
    float y;
    float z;
} AttractorPosition_t;

typedef float (*fn_strange_eval)( const float, const float[], const float, const float, const float);

typedef struct
{
    AttractorPosition_t position;
    float               step;
    float               speed;
    float               parameters[MAX_PARAMETERS];

    // Function pointers to attractor specific systems of equations
    fn_strange_eval fn_x;
    fn_strange_eval fn_y;
    fn_strange_eval fn_z;
} AttractorSystem_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_calc_point_at_t( float time, uint8_t variant, AttractorPosition_t *result_pos );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* ATTRACTOR_TYPES_H */
