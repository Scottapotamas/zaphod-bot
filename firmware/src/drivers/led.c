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

/* -------------------------------------------------------------------------- */

GenericColour_t setpoint = { 0 };
GenericColour_t corrected_setpoint = { 0 };
float speed_luma_factor = 1.0f;

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
led_update_speed_luma_factor( uint32_t mm_second )
{
    uint32_t max_speed = configuration_get_effector_speed_limit();

    // How fast are we moving relative to the max?
    float speed_factor = (float)mm_second / (float)max_speed;

    speed_luma_factor = CLAMP( speed_factor, 0.05f, 1.0f );
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

    if( configuration_get_led_speed_compensation_enabled() )
    {
        reduction_factor = speed_luma_factor;
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
