/* ----- System Includes ---------------------------------------------------- */

#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "attractor_types.h"
#include "qassert.h"

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

PRIVATE float lorenz_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float lorenz_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float lorenz_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float thomas_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float thomas_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float thomas_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float aizawa_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float aizawa_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float aizawa_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float dadras_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float dadras_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float dadras_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float chen_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float chen_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float chen_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float rossler_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float rossler_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float rossler_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float halvorsen_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float halvorsen_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float halvorsen_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float rabinovich_fabrikant_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float rabinovich_fabrikant_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float rabinovich_fabrikant_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float three_scroll_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float three_scroll_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float three_scroll_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float sprott_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float sprott_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float sprott_z( const float speed, const float parameters[], const float x, const float y, const float z );

PRIVATE float four_wing_x( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float four_wing_y( const float speed, const float parameters[], const float x, const float y, const float z );
PRIVATE float four_wing_z( const float speed, const float parameters[], const float x, const float y, const float z );

/* -------------------------------------------------------------------------- */

// Based on https://www.dynamicmath.xyz/strange-attractors
AttractorSystem_t defaults[MAX_VARIANTS] = {
    [LORENZ] = {
        .step = 0.009f,
        .speed = 1.0f,
        .parameters[PARAM_A] = 10.0f, // sigma
        .parameters[PARAM_B] = 2.6666666f, // beta 8/3 approx
        .parameters[PARAM_C] = 28.0f, // rho
        .fn_x = &lorenz_x,
        .fn_y = &lorenz_y,
        .fn_z = &lorenz_z,
    },
    [THOMAS] = {
        .step = 0.027f,
        .speed = 4.0f,
        .parameters[PARAM_B] = 0.208186f,
        .fn_x = &thomas_x,
        .fn_y = &thomas_y,
        .fn_z = &thomas_z,
    },
    [AIZAWA] = {
        .step = 0.01f,
        .speed = 2.0f,
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
        .step = 0.01f,
        .speed = 1.0f,
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
        .step = 0.05f,
        .speed = 1.0f,
        .parameters[PARAM_A] = 5.0f,
        .parameters[PARAM_B] = -10.0f,
        .parameters[PARAM_D] = -0.38f,
        .fn_x = &chen_x,
        .fn_y = &chen_y,
        .fn_z = &chen_z,
    },
    [ROSSLER] = {
        .step = 0.01f,
        .speed = 1.0f,
        .parameters[PARAM_A] = 0.2f,
        .parameters[PARAM_B] = 0.2f,
        .parameters[PARAM_C] = 5.7f,
        .fn_x = &rossler_x,
        .fn_y = &rossler_y,
        .fn_z = &rossler_z,
    },
    [HALVORSEN] = {
        .step = 0.01f,
        .speed = 1.0f,
        .parameters[PARAM_A] = 1.89f,
        .fn_x = &halvorsen_x,
        .fn_y = &halvorsen_y,
        .fn_z = &halvorsen_z,
    },
    [RABINOVICHFABRIKANT] = {
        .step = 0.05f,
        .speed = 1.0f,
        .parameters[PARAM_A] = 0.14f,
        .parameters[PARAM_C] = 0.1f, // gamma
        .fn_x = &rabinovich_fabrikant_x,
        .fn_y = &rabinovich_fabrikant_y,
        .fn_z = &rabinovich_fabrikant_z,
    },
    [THREESCROLL] = {
        .step = 0.0007f,
        .speed = 1.0f,
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
        .step = 0.01f,
        .speed = 3.0f,
        .parameters[PARAM_A] = 2.07f,
        .parameters[PARAM_B] = 1.79f,
        .fn_x = &sprott_x,
        .fn_y = &sprott_y,
        .fn_z = &sprott_z,
    },
    [FOURWING] = {
        .step = 0.02f,
        .speed = 3.0f,
        .parameters[PARAM_A] = 0.2f,
        .parameters[PARAM_B] = 0.01f,
        .parameters[PARAM_C] = -0.4f,
        .fn_x = &four_wing_x,
        .fn_y = &four_wing_y,
        .fn_z = &four_wing_z,
    },
};

/* -------------------------------------------------------------------------- */

PUBLIC AttractorSystem_t *
attractor_get_settings_ptr( uint8_t variant )
{
    REQUIRE( variant < MAX_VARIANTS);

    return &defaults[variant];
}

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_single_step( const AttractorSystem_t *state, AttractorPosition_t *current, AttractorPosition_t *result )
{
    REQUIRE( state );
    REQUIRE( current );
    REQUIRE( result );

    float nx = state->fn_x( state->speed, state->parameters, current->x, current->y, current->z );
    float ny = state->fn_y( state->speed, state->parameters, current->x, current->y, current->z );
    float nz = state->fn_z( state->speed, state->parameters, current->x, current->y, current->z );

    result->x = current->x + ( state->step * nx );
    result->y = current->y + ( state->step * ny );
    result->z = current->z + ( state->step * nz );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
attractor_runge_kutta( const AttractorSystem_t *state, AttractorPosition_t current, AttractorPosition_t *result )
{
    REQUIRE( state );
    // TODO pass the current position in via ptr and assert if invalid?
    REQUIRE( result );

    // Common values
    float h2 = 0.5f * state->step;
    float h6 = state->step / 6.0f;
//    float h2_time = time + h2;

    float speed = state->speed;

    // First pass
    float k1 = state->fn_x( speed, state->parameters, current.x, current.y, current.z );
    float j1 = state->fn_y( speed, state->parameters, current.x, current.y, current.z );
    float i1 = state->fn_z( speed, state->parameters, current.x, current.y, current.z );

    float h2_k1 = h2 * k1;
    float h2_j1 = h2 * j1;
    float h2_i1 = h2 * i1;

    float k2 = state->fn_x( speed, state->parameters, current.x + h2_k1, current.y + h2_j1, current.z + h2_i1 );
    float j2 = state->fn_y( speed, state->parameters, current.x + h2_k1, current.y + h2_j1, current.z + h2_i1 );
    float i2 = state->fn_z( speed, state->parameters, current.x + h2_k1, current.y + h2_j1, current.z + h2_i1 );

    float h2_k2 = h2 * k2;
    float h2_j2 = h2 * j2;
    float h2_i2 = h2 * i2;

    float k3 = state->fn_x( speed, state->parameters, current.x + h2_k2, current.y + h2_j2, current.z + h2_i2 );
    float j3 = state->fn_y( speed, state->parameters, current.x + h2_k2, current.y + h2_j2, current.z + h2_i2 );
    float i3 = state->fn_z( speed, state->parameters, current.x + h2_k2, current.y + h2_j2, current.z + h2_i2 );

    float h_k3 = state->step * k3;
    float h_j3 = state->step * j3;
    float h_i3 = state->step * i3;

    float k4 = state->fn_x( speed, state->parameters, current.x + h_k3, current.y + h_j3, current.z + h_i3 );
    float j4 = state->fn_y( speed, state->parameters, current.x + h_k3, current.y + h_j3, current.z + h_i3 );
    float i4 = state->fn_z( speed, state->parameters, current.x + h_k3, current.y + h_j3, current.z + h_i3 );

    // Output
    result->x = current.x + h6 * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
    result->y = current.y + h6 * (j1 + 2.0f * j2 + 2.0f * j3 + j4);
    result->z = current.z + h6 * (i1 + 2.0f * i2 + 2.0f * i3 + i4);
}

/* -------------------------------------------------------------------------- */

// Lorenz E. N. (1963).
// Deterministic Nonperiodic Flow.
// Journal of the Atmospheric Sciences. 20(2): 130–141.

PRIVATE float lorenz_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.5f * speed * ( parameters[PARAM_C] * ( -1.0f*x + y) );
}

PRIVATE float lorenz_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.5f * speed * ( -x * z + parameters[PARAM_A] * x - y );
}

PRIVATE float lorenz_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.5f * speed * ( x * y - parameters[PARAM_B] * z );
}

/* -------------------------------------------------------------------------- */

PRIVATE float thomas_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * sinf( y ) - parameters[PARAM_B] * x;
}

PRIVATE float thomas_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * sinf( z ) - parameters[PARAM_B] * y;
}

PRIVATE float thomas_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * sinf( y ) - parameters[PARAM_B] * z;
}

/* -------------------------------------------------------------------------- */

PRIVATE float aizawa_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( (z - parameters[PARAM_B]) * x - parameters[PARAM_D] * y );
}

PRIVATE float aizawa_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_D] * x + (z - parameters[PARAM_B]) * y );
}

PRIVATE float aizawa_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_C] + parameters[PARAM_A] * z - ( z*z*z / 3.0f )
                     - ( x * x + y * y ) * ( 1.0f + parameters[PARAM_E] * z)
                     + parameters[PARAM_F] * z * x * x * x );
}

/* -------------------------------------------------------------------------- */

// Dadras, S., Momeni, H.R. (2009).
// A novel three-dimensional autonomous chaotic system generating two, three and four-scroll attractors.
// Physics Letters A. Volume 373, Issue 40. pp. 3637-3642.

PRIVATE float dadras_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( y - parameters[PARAM_A] * x + parameters[PARAM_B] * y * z );
}

PRIVATE float dadras_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_C] * y - x * z + z );
}

PRIVATE float dadras_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_D] * x * y - parameters[PARAM_E] * z );
}

/* -------------------------------------------------------------------------- */

// Tam L., Chen J., Chen H., Tou W. (2008).
// Generation of hyperchaos from the Chen–Lee system via sinusoidal perturbation.
// Chaos, Solitons and Fractals. Vol. 38, 826-839

PRIVATE float chen_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_A] * x - y * z );
}

PRIVATE float chen_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_B] * y + x * z );
}

PRIVATE float chen_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_D] * z + x * y/3.0f );
}

/* -------------------------------------------------------------------------- */

// Rössler, O. E. (1976).
// An Equation for Continuous Chaos.
// Physics Letters, 57A (5): 397–398.

PRIVATE float rossler_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( -1.0f * ( y + z ) );
}

PRIVATE float rossler_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( x + parameters[PARAM_A] * y );
}

PRIVATE float rossler_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_B] + z * ( x - parameters[PARAM_C] ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE float halvorsen_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.3f * speed * ( -1.0f * parameters[PARAM_A] * x - 4.0f * y - 4.0f * z - y * y );
}

PRIVATE float halvorsen_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.3f * speed * ( -1.0f * parameters[PARAM_A] * y - 4.0f * z - 4.0f * x - z * z );
}

PRIVATE float halvorsen_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.3f * speed * ( -1.0f * parameters[PARAM_A] * z - 4.0f * x - 4.0f * y - x * x );
}

/* -------------------------------------------------------------------------- */

PRIVATE float rabinovich_fabrikant_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    // check gamma is c?
    return 0.2f * speed * ( y * ( z - 1.0f + x * x ) + parameters[PARAM_C] * x );
}

PRIVATE float rabinovich_fabrikant_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.2f * speed * ( x * ( 3.0f * z + 1 - x * x ) + parameters[PARAM_C] * y );
}

PRIVATE float rabinovich_fabrikant_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 0.2f * speed * ( -2.0f * z * ( parameters[PARAM_A] + x * y ) );
}

/* -------------------------------------------------------------------------- */

// Tscus
PRIVATE float three_scroll_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 1.1f * speed * ( parameters[PARAM_A] * (x - y) + parameters[PARAM_D] * x * z );
}

PRIVATE float three_scroll_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 1.1f * speed * ( parameters[PARAM_B] * x - x * z + parameters[PARAM_F] * y );
}

PRIVATE float three_scroll_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return 1.1f * speed * ( parameters[PARAM_C] * z + x * y - parameters[PARAM_E] * x * x );
}

/* -------------------------------------------------------------------------- */

// Sprott. J. C. (2014).
// A dynamical system with a strange attractor and invariant tori
// Physic Letters A, 378 1361-1363.

PRIVATE float sprott_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( y + parameters[PARAM_A] * x * y + x * z );
}

PRIVATE float sprott_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( 1.0f - parameters[PARAM_B] * x * x + y * z );
}

PRIVATE float sprott_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( x - x * x - y * y );
}

/* -------------------------------------------------------------------------- */

// Wang, Z., Sun, Y., van Wyk, J. B, Qi, G, van Wyk, M. A. (2009).
// A 3-D four-wing attractor and its analysis.
// Brazilian Journal of Physics, vol. 39, no. 3. pp.547-553

PRIVATE float four_wing_x( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_A] * x + y * z );
}

PRIVATE float four_wing_y( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( parameters[PARAM_B] * x + parameters[PARAM_C] * y - x * z );
}

PRIVATE float four_wing_z( const float speed, const float parameters[], const float x, const float y, const float z )
{
    return speed * ( -1.0f * z - x * y );
}

/* ----- End ---------------------------------------------------------------- */
