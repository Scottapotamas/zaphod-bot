/* -------------------------------------------------------------------------- */

#include <math.h>
#include <string.h>

#include "led.h"
#include "qassert.h"

#include "hal_gpio.h"
#include "hal_pwm.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

#define LED_FREQUENCY_HZ (8000U)   // TODO check timer output is accurate/prescale correct

/* -------------------------------------------------------------------------- */

PRIVATE void led_event_callback( ObserverEvent_t event, EventData eData, void *context );

/* -------------------------------------------------------------------------- */

PRIVATE GenericColour_t requested_setpoint = { 0 };
PRIVATE GenericColour_t corrected_setpoint = { 0 };

PRIVATE bool request_needs_processing = false;

bool compensate_luma_for_speed = false;
bool positional_luma_noise = false;

float speed_luma_factor = 1.0f;
float positional_noise_luma_factor = 1.0f;


PRIVATE Observer led_observer;

PRIVATE SemaphoreHandle_t xNewSetpointSemaphore;
PRIVATE SemaphoreHandle_t xLEDMutex;

/* -------------------------------------------------------------------------- */

PUBLIC void led_init( void )
{
    hal_pwm_generation( _PWM_TIM_AUX_0, LED_FREQUENCY_HZ );
    hal_pwm_generation( _PWM_TIM_AUX_1, LED_FREQUENCY_HZ );
    hal_pwm_generation( _PWM_TIM_AUX_2, LED_FREQUENCY_HZ );

    xNewSetpointSemaphore = xSemaphoreCreateBinary();
    xLEDMutex = xSemaphoreCreateMutex();

    ENSURE( xNewSetpointSemaphore );
    ENSURE( xLEDMutex );

    // Setup sensor event subscriptions
    observer_init( &led_observer, led_event_callback, NULL );

    observer_subscribe( &led_observer, EFFECTOR_POSITION );
    observer_subscribe( &led_observer, EFFECTOR_SPEED );

}

/* -------------------------------------------------------------------------- */

PUBLIC Observer * led_get_observer( void )
{
    return &led_observer;
}

/* -------------------------------------------------------------------------- */

PRIVATE void led_event_callback( ObserverEvent_t event, EventData eData, void *context )
{
    // TODO: is a mutex on the data needed to prevent races?
    switch( event )
    {
        case EFFECTOR_POSITION:
            // TODO: handle/update positional noise modifier
            xSemaphoreGive( xNewSetpointSemaphore );
            break;

        case EFFECTOR_SPEED:
            // TODO: handle speed-compensated brightness modifier
            xSemaphoreGive( xNewSetpointSemaphore );
            break;

        default:
            ASSERT( false );
            break;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void led_request_target( HSIColour_t *colour )
{
    REQUIRE( colour );

    // Ask for access to the effector data
    if( xSemaphoreTake( xLEDMutex, portMAX_DELAY ) )
    {
        // Is the request a new position
        if( memcmp( &requested_setpoint, colour, sizeof(GenericColour_t) ) != 0 )
        {
            memcpy( &requested_setpoint, colour, sizeof(GenericColour_t) );
            request_needs_processing = true;

            xSemaphoreGive( xLEDMutex );

            // Notify the task of the new colour
            xSemaphoreGive( xNewSetpointSemaphore );
        }
        else
        {
            xSemaphoreGive( xLEDMutex );
        }
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void led_task( void* arg )
{
    for(;;)
    {
        if( xSemaphoreTake( xNewSetpointSemaphore, portMAX_DELAY) )
        {
            // Gain access to state data
            if( xSemaphoreTake(xLEDMutex, portMAX_DELAY) )
            {
                // Post-process a new setpoint
                // only needs to be applied 'once' when the setpoint changes
                if( request_needs_processing )
                {
                    // Apply white-balance corrections

                    corrected_setpoint.x = requested_setpoint.x;
                    corrected_setpoint.y = requested_setpoint.y;
                    corrected_setpoint.z = requested_setpoint.z;

                    request_needs_processing = false;
                }

                // Calculate positional noise
                // TODO re-implement positional noise

                // Apply speed-based luma compensation
                // TODO re-implement speed compensation


                // Apply a luma linearisation
                // TODO revisit gamma curves

                GenericColour_t output = { 0 };

                // Ensure values are still valid
                // TODO: consider detecting/warning if clipping is detected?
                output.x = CLAMP( output.x, 0.0f, 1.0f );
                output.y = CLAMP( output.y, 0.0f, 1.0f );
                output.z = CLAMP( output.z, 0.0f, 1.0f );

                // TODO: consider detecting situations where the driver can be disabled entirely
                // Enable/disable the driver
    //            hal_gpio_write_pin( _AUX_ANALOG_0, enable );

                // PWM peripheral expects 0-100 percentage input, also invert the polarity of the duty cycle
                output.x = ( output.x * -1.0f + 1.0f ) * 100.0f;
                output.y = ( output.y * -1.0f + 1.0f ) * 100.0f;
                output.z = ( output.z * -1.0f + 1.0f ) * 100.0f;

                // Set the output duty cycles for the LED PWM channels
                hal_pwm_set_percentage_f( _PWM_TIM_AUX_0, output.x );
                hal_pwm_set_percentage_f( _PWM_TIM_AUX_1, output.y );
                hal_pwm_set_percentage_f( _PWM_TIM_AUX_2, output.z );

                xSemaphoreGive(xLEDMutex);
            }
        }   // end semph take
    }   // end task infinite loop
}

/* -------------------------------------------------------------------------- */

// Calculate a brightness reduction factor based on the current movement speed
// This should result in equivalent exposure for lines rendered at different speeds
// Result is 1.0 when moving at maximum speed down to a clamped value of 0.05 (5%)

PRIVATE void
led_calculate_speed_luma_factor( uint32_t microns_second )
{
    // TODO: get configurable maxSpeed value?
    uint32_t max_speed = 600U; //configuration_get_effector_speed_limit();

    // How fast are we moving relative to the max?
    float speed_factor = ((float)microns_second / 1000) / (float)max_speed;

    speed_luma_factor = CLAMP( speed_factor, 0.05f, 1.0f );
}

/* -------------------------------------------------------------------------- */

// For a given position in 3D space, calculate a noise-based luma offset
//PUBLIC void
//led_update_positional_noise( CartesianPoint_t position )
//{
//    // Normalise the position of the effector to a percentage of the allowable volume
//    float radius_mm = 200.0f;  //(float)configuration_get_volume_restriction_radius_mm();
//    float range_z = 180.0f;  //(float)configuration_get_volume_restriction_height_mm();
//
//    float normalised_x = MAP( MICRONS_TO_MM( (float)position.x ), -1*radius_mm, radius_mm, 0.0f, 1.0f );
//    float normalised_y = MAP( MICRONS_TO_MM( (float)position.y ), -1*radius_mm, radius_mm, 0.0f, 1.0f );
//    float normalised_z = MAP( MICRONS_TO_MM( (float)position.z ), 0.0f, range_z, 0.0f, 1.0f );
//
//    // Repeat the behaviour below for each axis
//    float nx = fbm( normalised_x );
//    float ny = fbm( normalised_y );
//    float nz = fbm( normalised_z );
//
//    positional_noise_luma_factor = ( nx + ny + nz ) / 3;
//}

/* -------------------------------------------------------------------------- */

//PRIVATE void
//led_whitebalance_correct( float *red, float *green, float *blue )
//{
//    // Whitebalance corrections can only reduce the strength of a LED
//    // as correcting 'brighter' doesn't make sense if a given channel is at max
//    uint16_t wb_r, wb_g, wb_b = 0;
////    configuration_get_led_whitebalance( &wb_r, &wb_g, &wb_b );
//
//    // Apply offsets
//    *red   = *red * ( 1.0f - ( (float)wb_r / 0xFFFFU ) );
//    *green = *green * ( 1.0f - ( (float)wb_g / 0xFFFFU ) );
//    *blue  = *blue * ( 1.0f - ( (float)wb_b / 0xFFFFU ) );
//}

/* -------------------------------------------------------------------------- */