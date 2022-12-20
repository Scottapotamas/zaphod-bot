/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "attractor_types.h"

#include "app_events.h"
#include "app_signals.h"
#include "event_subscribe.h"

/* -------------------------------------------------------------------------- */

PRIVATE float lorenz_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float lorenz_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float lorenz_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float thomas_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float thomas_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float thomas_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float aizawa_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float aizawa_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float aizawa_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float dadras_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float dadras_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float dadras_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float chen_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float chen_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float chen_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float rossler_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float rossler_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float rossler_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float halvorsen_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float halvorsen_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float halvorsen_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float rabinovich_fabrikant_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float rabinovich_fabrikant_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float rabinovich_fabrikant_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float three_scroll_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float three_scroll_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float three_scroll_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float sprott_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float sprott_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float sprott_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float four_wing_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float four_wing_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float four_wing_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z );

/* -------------------------------------------------------------------------- */

// Based on https://www.dynamicmath.xyz/strange-attractors
AttractorSystem_t defaults[MAX_VARIANTS] = {
    [LORENZ] = {
        .parameters[PARAM_A] = 10.0f, // sigma
        .parameters[PARAM_B] = 2.6666666f, // beta 8/3 approx
        .parameters[PARAM_C] = 28.0f, // rho
        .fn_x = &lorenz_x,
        .fn_y = &lorenz_y,
        .fn_z = &lorenz_z,
    },
    [THOMAS] = {
        .parameters[PARAM_B] = 0.208186f,
        .fn_x = &thomas_x,
        .fn_y = &thomas_y,
        .fn_z = &thomas_z,
    },
    [AIZAWA] = {
        .parameters[PARAM_A] = 0.95f,
        .parameters[PARAM_B] = 0.7f,
        .parameters[PARAM_C] = 0.6f,
        .parameters[PARAM_D] = 3.5f,
        .parameters[PARAM_E] = 0.25f,
        .parameters[PARAM_F] = 0.1f,
        .fn_x = &aizawa_x,
        .fn_y = &aizawa_y,
        .fn_z = &aizawa_z,
    },
    [DADRAS] = {
        .parameters[PARAM_A] = 3.0f,
        .parameters[PARAM_B] = 2.7f,
        .parameters[PARAM_C] = 1.7f,
        .parameters[PARAM_D] = 2.0f,
        .parameters[PARAM_E] = 9.0f,
        .fn_x = &dadras_x,
        .fn_y = &dadras_y,
        .fn_z = &dadras_z,
    },
    [CHEN] = {
        .parameters[PARAM_A] = 5.0f,
        .parameters[PARAM_B] = -10.0f,
        .parameters[PARAM_D] = -0.38f,
        .fn_x = &chen_x,
        .fn_y = &chen_y,
        .fn_z = &chen_z,
    },
    [ROSSLER] = {
        .parameters[PARAM_A] = 0.2f,
        .parameters[PARAM_B] = 0.2f,
        .parameters[PARAM_C] = 5.7f,
        .fn_x = &rossler_x,
        .fn_y = &rossler_y,
        .fn_z = &rossler_z,
    },
    [HALVORSEN] = {
        .parameters[PARAM_A] = 1.89f,
        .fn_x = &halvorsen_x,
        .fn_y = &halvorsen_y,
        .fn_z = &halvorsen_z,
    },
    [RABINOVICHFABRIKANT] = {
        .parameters[PARAM_A] = 0.14f,
        .parameters[PARAM_C] = 0.1f, // gamma
        .fn_x = &rabinovich_fabrikant_x,
        .fn_y = &rabinovich_fabrikant_y,
        .fn_z = &rabinovich_fabrikant_z,
    },
    [THREESCROLL] = {
        .parameters[PARAM_A] = 32.48f,
        .parameters[PARAM_B] = 45.84f,
        .parameters[PARAM_C] = 1.18f,
        .parameters[PARAM_D] = 0.13f,
        .parameters[PARAM_E] = 0.57f,
        .parameters[PARAM_F] = 14.7f,
        .fn_x = &three_scroll_x,
        .fn_y = &three_scroll_y,
        .fn_z = &three_scroll_z,
    },
    [SPROTT] = {
        .parameters[PARAM_A] = 2.07f,
        .parameters[PARAM_B] = 1.79f,
        .fn_x = &sprott_x,
        .fn_y = &sprott_y,
        .fn_z = &sprott_z,
    },
    [FOURWING] = {
        .parameters[PARAM_A] = 0.2f,
        .parameters[PARAM_B] = 0.01f,
        .parameters[PARAM_C] = -0.4f,
        .fn_x = &four_wing_x,
        .fn_y = &four_wing_y,
        .fn_z = &four_wing_z,
    },
};

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_init( void )
{

}

/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

PRIVATE void attractor_runge_kutta( const float time, const AttractorSystem_t *state, AttractorPosition_t *result_pos )
{
    // Common values
    float h2 = 0.5f * state->height;
    float h6 = state->height / 6.0f;
    float h2_time = time + h2;

    float height = 0.1f;

    // First pass
    float k1 = state->fn_x( time, height, state->parameters, state->position.x, state->position.y, state->position.z );
    float j1 = state->fn_y( time, height, state->parameters, state->position.x, state->position.y, state->position.z );
    float i1 = state->fn_z( time, height, state->parameters, state->position.x, state->position.y, state->position.z );

    float h2_k1 = h2 * k1;
    float h2_j1 = h2 * j1;
    float h2_i1 = h2 * i1;

    float k2 = state->fn_x( h2_time, height, state->parameters, state->position.x + h2_k1, state->position.y + h2_j1, state->position.z + h2_i1 );
    float j2 = state->fn_y( h2_time, height, state->parameters, state->position.x + h2_k1, state->position.y + h2_j1, state->position.z + h2_i1 );
    float i2 = state->fn_z( h2_time, height, state->parameters, state->position.x + h2_k1, state->position.y + h2_j1, state->position.z + h2_i1 );

    float h2_k2 = h2 * k1;
    float h2_j2 = h2 * j1;
    float h2_i2 = h2 * i1;

    float k3 = state->fn_x( h2_time, height, state->parameters, state->position.x + h2_k2, state->position.y + h2_j2, state->position.z + h2_i2 );
    float j3 = state->fn_y( h2_time, height, state->parameters, state->position.x + h2_k2, state->position.y + h2_j2, state->position.z + h2_i2 );
    float i3 = state->fn_z( h2_time, height, state->parameters, state->position.x + h2_k2, state->position.y + h2_j2, state->position.z + h2_i2 );

    float h_k3 = state->height * k3;
    float h_j3 = state->height * j3;
    float h_i3 = state->height * i3;

    float k4 = state->fn_x( time + state->height, height, state->parameters, state->position.x + h_k3, state->position.y + h_j3, state->position.z + h_i3 );
    float j4 = state->fn_y( time + state->height, height, state->parameters, state->position.x + h_k3, state->position.y + h_j3, state->position.z + h_i3 );
    float i4 = state->fn_z( time + state->height, height, state->parameters, state->position.x + h_k3, state->position.y + h_j3, state->position.z + h_i3 );

    // Output
    result_pos->x = state->position.x + h6 * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
    result_pos->y = state->position.y + h6 * (j1 + 2.0f * j2 + 2.0f * j3 + j4);
    result_pos->z = state->position.z + h6 * (i1 + 2.0f * i2 + 2.0f * i3 + i4);
}

/* -------------------------------------------------------------------------- */

PRIVATE float lorenz_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.5f * speed * ( parameters[PARAM_C] * ( -1.0f*x + y) );
}

PRIVATE float lorenz_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.5f * speed * ( -x * z + parameters[PARAM_A] * x - y );
}

PRIVATE float lorenz_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.5f * speed * ( x * y - parameters[PARAM_B] * z );
}

/* -------------------------------------------------------------------------- */

PRIVATE float thomas_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * sinf( y ) - parameters[PARAM_B] * x;
}

PRIVATE float thomas_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * sinf( z ) - parameters[PARAM_B] * y;
}

PRIVATE float thomas_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * sinf( y ) - parameters[PARAM_B] * z;
}

/* -------------------------------------------------------------------------- */

PRIVATE float aizawa_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( (z - parameters[PARAM_B]) * x - parameters[PARAM_D] * y );
}

PRIVATE float aizawa_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_D] * x + (z - parameters[PARAM_B]) * y );
}

PRIVATE float aizawa_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_C] + parameters[PARAM_A] * z - ( z*z*z / 3.0f )
                     - ( x * x + y * y ) * ( 1.0f + parameters[PARAM_E] * z)
                     + parameters[PARAM_F] * z * x * x * x );
}

/* -------------------------------------------------------------------------- */

PRIVATE float dadras_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( y - parameters[PARAM_A] * x + parameters[PARAM_B] * y * z );
}

PRIVATE float dadras_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_C] * y - x * z + z );
}

PRIVATE float dadras_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_D] * x * y - parameters[PARAM_E] * z );
}

/* -------------------------------------------------------------------------- */

PRIVATE float chen_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_A] * x - y * z );
}

PRIVATE float chen_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_B] * y + x * z );
}

PRIVATE float chen_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_D] * z + x * y/3.0f );
}

/* -------------------------------------------------------------------------- */

PRIVATE float rossler_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( -1.0f * ( y + z ) );
}

PRIVATE float rossler_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( x + parameters[PARAM_A] * y );
}

PRIVATE float rossler_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_B] + z * ( x - parameters[PARAM_C] ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE float halvorsen_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.3f * speed * ( -1.0f * parameters[PARAM_A] * x - 4.0f * y - 4.0f * z - y * y );
}

PRIVATE float halvorsen_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.3f * speed * ( -1.0f * parameters[PARAM_A] * y - 4.0f * z - 4.0f * x - z * z );
}

PRIVATE float halvorsen_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.3f * speed * ( -1.0f * parameters[PARAM_A] * z - 4.0f * x - 4.0f * y - x * x );
}

/* -------------------------------------------------------------------------- */

PRIVATE float rabinovich_fabrikant_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    // check gamma is c?
    return 0.2f * speed * ( y * ( z - 1.0f + x * x ) + parameters[PARAM_C] * x );
}

PRIVATE float rabinovich_fabrikant_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.2f * speed * ( x * ( 3.0f * z + 1 - x * x ) + parameters[PARAM_C] * y );
}

PRIVATE float rabinovich_fabrikant_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.2f * speed * ( -2.0f * z * ( parameters[PARAM_A] + x * y ) );
}

/* -------------------------------------------------------------------------- */

// Tscus
PRIVATE float three_scroll_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 1.1f * speed * ( parameters[PARAM_A] * (x - y) + parameters[PARAM_D] * x * z );
}

PRIVATE float three_scroll_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 1.1f * speed * ( parameters[PARAM_B] * x - x * z + parameters[PARAM_F] * y );
}

PRIVATE float three_scroll_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 1.1f * speed * ( parameters[PARAM_C] * z + x * y - parameters[PARAM_E] * x * x );
}

/* -------------------------------------------------------------------------- */

PRIVATE float sprott_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( y + parameters[PARAM_A] * x * y + x * z );
}

PRIVATE float sprott_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( 1.0f - parameters[PARAM_B] * x * x + y * z );
}

PRIVATE float sprott_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( x - x * x - y * y );
}

/* -------------------------------------------------------------------------- */

// Wang-Sun

PRIVATE float four_wing_x( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_A] * x + y * z );
}

PRIVATE float four_wing_y( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_B] * x + parameters[PARAM_C] * y - x * z );
}

PRIVATE float four_wing_z( const float time, const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( -1.0f * z - x * y );
}

/* ----- End ---------------------------------------------------------------- */
