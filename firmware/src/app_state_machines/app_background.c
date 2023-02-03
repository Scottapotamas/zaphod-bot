/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "app_background.h"
#include "app_times.h"
#include "global.h"
#include "timer_ms.h"

#include "button.h"
#include "buzzer.h"
#include "clearpath.h"
#include "fan.h"
#include "hal_adc.h"
#include "hal_system_speed.h"
#include "sensors.h"
#include "status.h"

#include "user_interface.h"

/* -------------------------------------------------------------------------- */

PRIVATE timer_ms_t button_timer = 0;
PRIVATE timer_ms_t buzzer_timer = 0;
PRIVATE timer_ms_t fan_timer    = 0;
PRIVATE timer_ms_t adc_timer    = 0;

/* -------------------------------------------------------------------------- */

PUBLIC void
app_background_init( void )
{
    timer_ms_start( &button_timer, BACKGROUND_RATE_BUTTON_MS );
    timer_ms_start( &buzzer_timer, BACKGROUND_RATE_BUZZER_MS );
    timer_ms_start( &fan_timer, FAN_EVALUATE_TIME );
    timer_ms_start( &adc_timer, BACKGROUND_ADC_AVG_POLL_MS );    // refresh ADC readings
}

/* -------------------------------------------------------------------------- */

PUBLIC void
app_background( void )
{
    // rate limit less important background processes
    if( timer_ms_is_expired( &button_timer ) )
    {
        // Need to turn the E-Stop light on to power the pull-up for the E-STOP button
        status_external_override( true );
        button_process();
        status_external_resume();

        user_interface_push_position();
        timer_ms_start( &button_timer, BACKGROUND_RATE_BUTTON_MS );
    }

    if( timer_ms_is_expired( &buzzer_timer ) )
    {
        buzzer_process();
        timer_ms_start( &buzzer_timer, BACKGROUND_RATE_BUZZER_MS );
    }

    if( timer_ms_is_expired( &fan_timer ) )
    {
        fan_process();
        user_interface_set_fan_percentage( fan_get_speed() );
        user_interface_set_fan_state( fan_get_state() );
        timer_ms_start( &fan_timer, FAN_EVALUATE_TIME );
    }

    if( timer_ms_is_expired( &adc_timer ) )
    {
        sensors_12v_regulator_C();
        sensors_ambient_C();
        sensors_expansion_C();
        sensors_microcontroller_C();
        sensors_input_V();

        user_interface_set_cpu_load( (uint8_t)hal_system_speed_get_load() );
        user_interface_set_cpu_clock( hal_system_speed_get_speed() );

        timer_ms_start( &adc_timer, BACKGROUND_ADC_AVG_POLL_MS );
    }

    for( ClearpathServoInstance_t servo = _CLEARPATH_1; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
    {
        servo_process( servo );
    }

    user_interface_handle_data();
}

/* ----- End ---------------------------------------------------------------- */
