/* ----- System Includes ---------------------------------------------------- */

#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "led.h"
#include "app_times.h"

#include "hal_gpio.h"
#include "hal_pwm.h"

#include "user_interface.h"
#include "configuration.h"

/* ----- Private Types ------------------------------------------------------ */

/* ----- Private Variables -------------------------------------------------- */

PRIVATE void led_apply_corrections( void );

PRIVATE float led_luminance_correct( float input );
PRIVATE void  led_whitebalance_correct( float *red, float *green, float *blue );
PRIVATE float led_power_limit();

PRIVATE float bnoise( float x );
PRIVATE float fbm( float value );

/* -------------------------------------------------------------------------- */

GenericColour_t setpoint = { 0 };
GenericColour_t corrected_setpoint = { 0 };
bool compensate_luma_for_speed = false;
bool positional_luma_noise = false;
float speed_luma_factor = 1.0f;
float positional_noise_luma_factor = 1.0f;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
led_init( void )
{
    hal_pwm_generation( _PWM_TIM_AUX_0, LED_FREQUENCY_HZ );
    hal_pwm_generation( _PWM_TIM_AUX_1, LED_FREQUENCY_HZ );
    hal_pwm_generation( _PWM_TIM_AUX_2, LED_FREQUENCY_HZ );

    // Ensure some PWM values are set right now
    led_refresh_output();
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_enable( bool enable )
{
    hal_gpio_write_pin( _AUX_ANALOG_0, enable );
    user_interface_set_led_status( enable );

    if( !enable )
    {
        user_interface_set_led_values( 0, 0, 0 );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_compensate_luma_for_speed( bool enable )
{
    compensate_luma_for_speed = enable;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_apply_positional_noise( bool enable )
{
    positional_luma_noise = enable;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_request_dark( void )
{
    setpoint.x = 0.0f;
    setpoint.y = 0.0f;
    setpoint.z = 0.0f;

    led_apply_corrections();
}

// Request RGB with 0.0 to 1.0 values
PUBLIC void
led_request_rgb( float r, float g, float b )
{
    // TODO: refactor to use a RGB float structure as arg instead
    setpoint.x = r;
    setpoint.y = g;
    setpoint.z = b;

    led_apply_corrections();
}

PUBLIC void
led_request_hsi( HSIColour_t color )
{
    // Convert to RGB
    hsi_to_rgb( color.hue, color.saturation, color.intensity, &setpoint.x, &setpoint.y, &setpoint.z );

    led_apply_corrections();
}

/* -------------------------------------------------------------------------- */

// Calculate a brightness reduction factor based on the current movement speed
// This should result in equivalent exposure for lines rendered at different speeds
// Result is 1.0 when moving at maximum speed down to a clamped value of 0.05 (5%)

PUBLIC void
led_update_speed_luma_factor( uint32_t microns_second )
{
    uint32_t max_speed = configuration_get_effector_speed_limit();

    // How fast are we moving relative to the max?
    float speed_factor = ((float)microns_second / 1000) / (float)max_speed;

    speed_luma_factor = CLAMP( speed_factor, 0.05f, 1.0f );
}

/* -------------------------------------------------------------------------- */

// 1D Noise function ported from https://www.shadertoy.com/view/3sd3Rs
// Fully deterministic, periodic and doesn't require caching/lookups like typical Perlin noise generators
PRIVATE float
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

// For a given position in 3D space, calculate a noise-based luma offset
PUBLIC void
led_update_positional_noise( CartesianPoint_t position )
{
    // Normalise the position of the effector to a percentage of the allowable volume
    float radius_mm = (float)configuration_get_volume_restriction_radius_mm();
    float range_z = (float)configuration_get_volume_restriction_height_mm();

    float normalised_x = MAP( MICRONS_TO_MM( (float)position.x ), -1*radius_mm, radius_mm, 0.0f, 1.0f );
    float normalised_y = MAP( MICRONS_TO_MM( (float)position.y ), -1*radius_mm, radius_mm, 0.0f, 1.0f );
    float normalised_z = MAP( MICRONS_TO_MM( (float)position.z ), 0.0f, range_z, 0.0f, 1.0f );

    // Repeat the behaviour below for each axis
    float nx = fbm( normalised_x );
    float ny = fbm( normalised_y );
    float nz = fbm( normalised_z );

    positional_noise_luma_factor = ( nx + ny + nz ) / 3;
}

/* -------------------------------------------------------------------------- */

// Applies luma and white-balance corrections
// These only need to be applied 'once' as the input setpoint changes
PRIVATE void
led_apply_corrections( void )
{
    if( configuration_get_led_luma_correction_enabled() )
    {
        corrected_setpoint.x = led_luminance_correct( setpoint.x );
        corrected_setpoint.y = led_luminance_correct( setpoint.y );
        corrected_setpoint.z = led_luminance_correct( setpoint.z );
    }
    else
    {
        corrected_setpoint.x = setpoint.x;
        corrected_setpoint.y = setpoint.y;
        corrected_setpoint.z = setpoint.z;
    }

    if( configuration_get_led_wb_correction_enabled() )
    {
        led_whitebalance_correct( &corrected_setpoint.x, &corrected_setpoint.y, &corrected_setpoint.z );
    }
}

/* -------------------------------------------------------------------------- */

// Requests PWM duty-cycles for the R, G, B outputs.
// Also apply the most recent effector speed compensation factor (if enabled)
PUBLIC void
led_refresh_output( void )
{
    GenericColour_t output = { 0 };
    float reduction_factor = 1.0f;

    if( compensate_luma_for_speed )
    {
        reduction_factor = speed_luma_factor;
    }

    if( positional_luma_noise )
    {
        reduction_factor += positional_noise_luma_factor;
    }

    output.x = corrected_setpoint.x * reduction_factor;
    output.y = corrected_setpoint.y * reduction_factor;
    output.z = corrected_setpoint.z * reduction_factor;

    output.x = CLAMP( output.x, 0.0f, 1.0f );
    output.y = CLAMP( output.y, 0.0f, 1.0f );
    output.z = CLAMP( output.z, 0.0f, 1.0f );

    // PWM peripheral expects 0-100 percentage input, also invert the polarity of the duty cycle
    output.x = ( output.x * -1.0f + 1.0f ) * 100.0f;
    output.y = ( output.y * -1.0f + 1.0f ) * 100.0f;
    output.z = ( output.z * -1.0f + 1.0f ) * 100.0f;

    // Set the output duty cycles for the LED PWM channels
    hal_pwm_set_percentage_f( _PWM_TIM_AUX_0, output.x );
    hal_pwm_set_percentage_f( _PWM_TIM_AUX_1, output.y );
    hal_pwm_set_percentage_f( _PWM_TIM_AUX_2, output.z );

    user_interface_set_led_values( output.x * 0xFFFF, output.y * 0xFFFF, output.z * 0xFFFF );
}

/* -------------------------------------------------------------------------- */

PRIVATE float
led_luminance_correct( float input )
{
    float lightness = input * 100;

    /* CIE 1931 Luminance Formula
     * https://en.wikipedia.org/wiki/CIE_1931_color_space
     *  Y = (L* / 902.3)           if L* ≤ 8
     *  Y = ((L* + 16) / 116)^3    if L* > 8
     */

    float luminance;

    if( lightness <= 8 )
    {
        luminance = ( lightness / 903.296f );
    }
    else
    {
        luminance = powf( ( ( lightness + 16 ) / 116 ), 3 );
    }

    return luminance;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
led_whitebalance_correct( float *red, float *green, float *blue )
{
    // Whitebalance corrections can only reduce the strength of a LED
    // as correcting 'brighter' doesn't make sense if a given channel is at max
    uint16_t wb_r, wb_g, wb_b = 0;
    configuration_get_led_whitebalance( &wb_r, &wb_g, &wb_b );

    // Apply offsets
    *red   = *red * ( 1.0f - ( (float)wb_r / 0xFFFFU ) );
    *green = *green * ( 1.0f - ( (float)wb_g / 0xFFFFU ) );
    *blue  = *blue * ( 1.0f - ( (float)wb_b / 0xFFFFU ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE float
led_power_limit( void )
{
    uint16_t limit = 0;
    configuration_get_led_bias( &limit );

    // Apply offset based on that value

    return 1.0f - ( (float)limit / 32767 );
}

/* ----- End ---------------------------------------------------------------- */
