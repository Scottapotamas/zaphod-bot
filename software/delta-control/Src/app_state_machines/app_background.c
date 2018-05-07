/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "app_background.h"
#include "event_subscribe.h"
#include "app_signals.h"
#include "button.h"
#include "hal_button.h"
#include "timer_ms.h"
#include "buzzer.h"
#include "fan.h"
#include "clearpath.h"

/* -------------------------------------------------------------------------- */

PRIVATE timer_ms_t button_timer = 0;

/* -------------------------------------------------------------------------- */

PUBLIC void
app_background_init( void )
{
	timer_ms_start( &button_timer, 50 );

}

/* -------------------------------------------------------------------------- */

PUBLIC void
app_background( void )
{
    if( timer_ms_is_expired( &button_timer ) )
    {
        if( button_pattern_match( BUTTON_PATTERN_EMERGENCY_SHUTDOWN ) )
        {
            //stop the motors now
        	//todo work out a ESTOP scheme that doesn't rely on a double-button combo

        }

    	timer_ms_start( &button_timer, 50 );
    }

    button_process();
    buzzer_process();
    fan_process();

    servo_process( _CLEARPATH_1 );
    servo_process( _CLEARPATH_2 );
    servo_process( _CLEARPATH_3 );

#ifdef EXPANSION_SERVO
    servo_process( _CLEARPATH_4 );
#endif

}

/* ----- End ---------------------------------------------------------------- */

