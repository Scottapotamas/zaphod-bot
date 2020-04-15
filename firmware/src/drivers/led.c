/* ----- System Includes ---------------------------------------------------- */

#include <math.h>
#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "app_times.h"
#include "configuration.h"
#include "hal_gpio.h"
#include "hal_pwm.h"
#include "led.h"

/* ----- Private Types ------------------------------------------------------ */

/* ----- Private Variables -------------------------------------------------- */

PRIVATE float
led_luminance_correct( float input );

PRIVATE void
led_whitebalance_correct( float *red, float *green, float *blue );

PRIVATE float
led_power_limit();

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
led_init( void )
{
    hal_pwm_generation( _PWM_TIM_AUX_0, LED_FREQUENCY_HZ );
    hal_pwm_generation( _PWM_TIM_AUX_1, LED_FREQUENCY_HZ );
    hal_pwm_generation( _PWM_TIM_AUX_2, LED_FREQUENCY_HZ );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_enable( bool enable )
{
    hal_gpio_write_pin( _AUX_ANALOG_0, enable );
    config_set_led_status( enable );

    if( !enable )
    {
        config_set_led_values( 0, 0, 0 );
    }
}

/* -------------------------------------------------------------------------- */

// Input 3 float [0-1] values for RGB color channels in linear space
// Applies luma and whitebalance correction
PUBLIC void
led_set( float r, float g, float b )
{
    float setpoint_r = led_luminance_correct( r );
    float setpoint_g = led_luminance_correct( g );
    float setpoint_b = led_luminance_correct( b );

    led_whitebalance_correct( &setpoint_r, &setpoint_g, &setpoint_b );
    setpoint_r *= led_power_limit();
    setpoint_g *= led_power_limit();
    setpoint_b *= led_power_limit();

    setpoint_r = CLAMP( setpoint_r, 0.0f, 1.0f );
    setpoint_g = CLAMP( setpoint_g, 0.0f, 1.0f );
    setpoint_b = CLAMP( setpoint_b, 0.0f, 1.0f );

    // Set the output duty cycles for the led PWM channels
    // PWM peripheral expects 0-100 percentage input, and we need to invert the polarity of the duty cycle
    hal_pwm_set_percentage_f( _PWM_TIM_AUX_0, ( setpoint_r * -1.0 + 1.0 ) * 100.0f );
    hal_pwm_set_percentage_f( _PWM_TIM_AUX_2, ( setpoint_g * -1.0 + 1.0 ) * 100.0f );
    hal_pwm_set_percentage_f( _PWM_TIM_AUX_1, ( setpoint_b * -1.0 + 1.0 ) * 100.0f );

    config_set_led_values( setpoint_r * 0xFFFF, setpoint_g * 0xFFFF, setpoint_b * 0xFFFF );
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

    float luminance = 0.0f;

    if( lightness <= 8 )
    {
        luminance = ( lightness / 903.296f );
    }
    else
    {
        luminance = pow( ( ( lightness + 16 ) / 116 ), 3 );
    }

    return luminance;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
led_whitebalance_correct( float *red, float *green, float *blue )
{
    int16_t wb_r, wb_g, wb_b = 0;
    config_get_led_whitebalance( &wb_r, &wb_g, &wb_b );

    // Apply offsets
    *red   = *red * ( 1.0 - ( (float)wb_r / 32767.0f ) );
    *green = *green * ( 1.0 - ( (float)wb_g / 32767.0f ) );
    *blue  = *blue * ( 1.0 - ( (float)wb_b / 32767.0f ) );
}

/* -------------------------------------------------------------------------- */

PRIVATE float
led_power_limit()
{
    int16_t limit = 0;
    config_get_led_bias( &limit );

    // Apply offset based on that value

    return 1.0 - ( (float)limit / 32767 );
}

/* ----- End ---------------------------------------------------------------- */
