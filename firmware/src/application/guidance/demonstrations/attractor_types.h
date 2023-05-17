#ifndef ATTRACTOR_TYPES_H
#define ATTRACTOR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

typedef enum {
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

typedef enum {
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
    float               step;
    float               speed;
    float               parameters[MAX_PARAMETERS];

    // Function pointers to attractor specific systems of equations
    fn_strange_eval fn_x;
    fn_strange_eval fn_y;
    fn_strange_eval fn_z;
} AttractorSystem_t;

/* -------------------------------------------------------------------------- */

PUBLIC AttractorSystem_t *
attractor_get_settings_ptr( uint8_t variant );

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_single_step( const AttractorSystem_t *state,
                       AttractorPosition_t *current,
                       AttractorPosition_t *result );

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_runge_kutta( const AttractorSystem_t *state,
                       AttractorPosition_t current,
                       AttractorPosition_t *result );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* ATTRACTOR_TYPES_H */