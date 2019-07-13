/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "led.h"
#include "app_times.h"
#include "hal_gpio.h"
#include "hal_pwm.h"
#include "configuration.h"

/* ----- Private Types ------------------------------------------------------ */

/* ----- Private Variables -------------------------------------------------- */


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
}

/* -------------------------------------------------------------------------- */

PUBLIC void
led_set( float r, float g, float b )
{
    uint8_t r_duty = r*255;
    uint8_t g_duty = g*255;
    uint8_t b_duty = b*255;

    //todo clamp led illumination power with override value

    //todo apply linear/log conversion for led

    //todo apply whitepoint offset for led

    // Set the final output percentages for the led PWM channels
    hal_pwm_set( _PWM_TIM_AUX_0, r_duty);
    hal_pwm_set( _PWM_TIM_AUX_2, g_duty);
    hal_pwm_set( _PWM_TIM_AUX_1, b_duty);
}

/* ----- End ---------------------------------------------------------------- */
