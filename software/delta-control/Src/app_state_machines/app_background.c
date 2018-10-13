/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "app_background.h"
#include "event_subscribe.h"
#include "app_signals.h"
#include "app_times.h"
#include "button.h"
#include "hal_button.h"
#include "timer_ms.h"
#include "buzzer.h"
#include "fan.h"
#include "path_interpolator.h"
#include "clearpath.h"

#include "status.h"
#include "hal_gpio.h"
#include "sensors.h"
#include "configuration.h"
#include "hal_system_speed.h"

#include "demo_move.h"
/* -------------------------------------------------------------------------- */

PRIVATE timer_ms_t 	button_timer 	= 0;
PRIVATE timer_ms_t 	buzzer_timer 	= 0;
PRIVATE timer_ms_t 	fan_timer 		= 0;
PRIVATE timer_ms_t 	adc_timer 		= 0;

/* -------------------------------------------------------------------------- */

PUBLIC void
app_background_init( void )
{
	timer_ms_start( &button_timer, 	BACKGROUND_RATE_BUTTON_MS );
	timer_ms_start( &buzzer_timer, 	BACKGROUND_RATE_BUZZER_MS );
	timer_ms_start( &fan_timer, 	FAN_EVALUATE_TIME );
	timer_ms_start( &adc_timer, 	250 );	//refresh ADC readings

}

/* -------------------------------------------------------------------------- */

PUBLIC void
app_background( void )
{
	//rate limit less important background processes
    if( timer_ms_is_expired( &button_timer ) )
    {
        if( button_pattern_match( BUTTON_PATTERN_EMERGENCY_SHUTDOWN ) )
        {
            //todo stop the motors now or something
        }

        button_process();
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
    	timer_ms_start( &fan_timer, FAN_EVALUATE_TIME );

#warning "Remove this horrid hack"
    sequence_event_handle();


    }

    if( timer_ms_is_expired( &adc_timer ) )
    {
        sensors_12v_regulator_C();
        sensors_ambient_C();
        sensors_expansion_C();
        sensors_microcontroller_C();

        sensors_input_V();

        config_set_cpu_load( hal_system_speed_get_load() );
        config_set_cpu_clock( hal_system_speed_get_speed() );

    	timer_ms_start( &adc_timer, 250 );
    }

    //process any running movements and allow servo drivers to process commands
    path_interpolator_process();
    servo_process( _CLEARPATH_1 );
    servo_process( _CLEARPATH_2 );
    servo_process( _CLEARPATH_3 );

#ifdef EXPANSION_SERVO
    servo_process( _CLEARPATH_4 );
#endif

}

/* ----- End ---------------------------------------------------------------- */

