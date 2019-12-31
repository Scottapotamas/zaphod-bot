/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "fan.h"
#include "simple_state_machine.h"
#include "hal_pwm.h"
#include "hal_hard_ic.h"
#include "hal_systick.h"
#include "sensors.h"
#include "app_times.h"
#include "configuration.h"

/* ----- Private Types ------------------------------------------------------ */

typedef enum
{
    FAN_STATE_OFF,
	FAN_STATE_STALL,
	FAN_STATE_START,
    FAN_STATE_ON,
} FanState_t;

typedef struct
{
	FanState_t   previousState;
	FanState_t   currentState;
	FanState_t   nextState;
    uint8_t      speed;			//as a percentage 0-100, what the fan should be at 'now'
    uint8_t      set_speed;		//as a percentage 0-100, requested speed target
    uint32_t     startup_timer;	//amount of time to 'blip' the fan for reliable starts
    bool         manual_control;
} Fan_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Fan_t fan;


PRIVATE FanCurve_t * fan_curve;

PRIVATE uint8_t
fan_speed_at_temp( float temperature );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
fan_init( void )
{
    memset( &fan, 0, sizeof( fan ) );

    //get a pointer to the fan curve configuration table
    fan_curve = config_get_fan_curve_ptr();
    hal_pwm_generation( _PWM_TIM_FAN, FAN_FREQUENCY_HZ );

    hal_setup_capture( _FAN_HALL );

    //todo setup input capture peripheral instead of relying on soft IC implementation
}

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_set( uint8_t speed_percentage )
{
	Fan_t *me = &fan;

	//0-100% speed control over fan
	me->set_speed = CLAMP( speed_percentage, 0, 100 );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_process( void )
{
	Fan_t *me = &fan;

	//get the current fan speed
	uint16_t fan_hall_rpm = sensors_fan_speed_RPM();
    me->manual_control = config_get_fan_manual_control();
    switch( me->currentState )
    {
        case FAN_STATE_OFF:
            STATE_ENTRY_ACTION

				//make sure fan is not spinning
				me->speed = 0;

            STATE_TRANSITION_TEST

				me->set_speed = fan_speed_at_temp( sensors_expansion_C() );

				//once new target is established, trigger startup blip
                if( me->set_speed > 0 )
                {
                    STATE_NEXT( FAN_STATE_START );
                }

            STATE_EXIT_ACTION
            STATE_END
            break;

        case FAN_STATE_STALL:
            STATE_ENTRY_ACTION
			//stop the fan because we think it's stalled
			me->speed = 0;
			me->startup_timer = hal_systick_get_ms();

			STATE_TRANSITION_TEST

			//check if the current setpoint is likely under the RPM stall limit
			if (me->set_speed < (FAN_STALL_MAX_RPM / FAN_STALL_FAULT_RPM))
			{
				me->set_speed += 10;	//increase the setpoint by 10%
			}

			//has timer expired
			if ((hal_systick_get_ms() - me->startup_timer) > FAN_STALL_WAIT_TIME_MS)
			{
				STATE_NEXT(FAN_STATE_START);
			}

            STATE_EXIT_ACTION
            STATE_END
            break;

        case FAN_STATE_START:
            STATE_ENTRY_ACTION
				//set PWM to 100% for configurable short period
        		me->speed = 100;
            	me->startup_timer = hal_systick_get_ms();

            STATE_TRANSITION_TEST
				//has the timer expired
            	if( (hal_systick_get_ms() - me->startup_timer) > FAN_STARTUP_TIME_MS )
            	{
            	    STATE_NEXT(FAN_STATE_ON );
            	}

            STATE_EXIT_ACTION
            STATE_END
            break;

        case FAN_STATE_ON:
            STATE_ENTRY_ACTION
				me->speed = me->set_speed;

            STATE_TRANSITION_TEST
                if( me->manual_control )
                {
                    // userspace 0-100% speed control over fan
                    me->set_speed = CLAMP( config_get_fan_target(), 0, 100 );
                }
                else
                {
                    // calculate target speed based on temperature reading
                    me->set_speed = fan_speed_at_temp( sensors_expansion_C() );
                }

				//speed change req while running
				if( me->set_speed != me->speed )
				{
					me->speed = me->set_speed;
				}

            	//fan req turned off
            	if( me->set_speed == 0 )
                {
                    STATE_NEXT( FAN_STATE_OFF );
                }

            	//rotor stop detection
            	if( fan_hall_rpm < FAN_STALL_FAULT_RPM )
            	{
            		//restart the fan
                    STATE_NEXT( FAN_STATE_STALL );
            	}

            STATE_EXIT_ACTION

            STATE_END
            break;
    }

	hal_pwm_set_percentage_f( _PWM_TIM_FAN, me->speed);
	config_set_fan_percentage( me->speed );
	config_set_fan_state( me->currentState );
}

/* -------------------------------------------------------------------------- */

// Calculate an appropriate fan speed percentage based on temperature lookup table.

PRIVATE uint8_t
fan_speed_at_temp( float temperature )
{
	//ensure the pointer is set (or default to a high fan speed)
	if(fan_curve)
	{
		//protect against out-of-bounds temperature inputs
	    if( temperature < fan_curve[ 0 ].temperature )
	    {
	    	//temperature is lower than lowest point in LUT
	        return fan_curve[ 0 ].percentage;
	    }
	    else if( temperature > fan_curve[ NUM_FAN_CURVE_POINTS - 1 ].temperature )
	    {
	    	//temperature exceeds max LUT value
	        return 100.0;
	    }

	    for( uint8_t i = 0; i < NUM_FAN_CURVE_POINTS - 1; i++ )
	    {
	    	//within range between two rows of the LUT
	        if( temperature > fan_curve[ i ].temperature && temperature <= fan_curve[ i + 1 ].temperature )
	        {
	        	//linear interpolation for fan speed between the surrounding rows in LUT
	        	return fan_curve[ i ].percentage + ( ((temperature - fan_curve[ i ].temperature)/(fan_curve[ i + 1 ].temperature - fan_curve[ i ].temperature)) * ( fan_curve[ i + 1 ].percentage - fan_curve[ i ].percentage ) ) ;
	        }
	    }
	}

    return 97;	// should have returned with a valid percentage by now, fail ON for safety.
}

/* ----- End ---------------------------------------------------------------- */
