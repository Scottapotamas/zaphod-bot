/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "fan.h"
#include "simple_state_machine.h"
#include "hal_pwm.h"
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
} Fan_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Fan_t fan;

typedef struct
{
    float temp;
    uint8_t percentage;
} FanControlPoint_t;

PRIVATE FanControlPoint_t fan_curve[] =
{
	{ .temp = 19.0f, 	.percentage =	0 },
    { .temp = 20.0f, 	.percentage =	30 },
    { .temp = 35.0f, 	.percentage =	50 },
    { .temp = 50.0f, 	.percentage =	60 },
    { .temp = 60.0f, 	.percentage =	80 },
    { .temp = 70.0f,	.percentage =	95 },
};

PRIVATE uint8_t
fan_speed_at_temp( float temperature );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
fan_init( void )
{
    memset( &fan, 0, sizeof( fan ) );

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

				//has timer expired
            	if( (hal_systick_get_ms() - me->startup_timer) > FAN_STARTUP_TIME_MS )
            	{
                    STATE_NEXT( FAN_STATE_ON );
            	}

            STATE_EXIT_ACTION
            STATE_END
            break;

        case FAN_STATE_ON:
            STATE_ENTRY_ACTION

				me->speed = me->set_speed;

            STATE_TRANSITION_TEST

				//recalculate target speed based on temperature reading
				me->set_speed = fan_speed_at_temp( sensors_expansion_C() );

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

	hal_pwm_generation( _PWM_TIM_FAN, FAN_FREQUENCY_HZ, me->speed );
	config_set_fan_percentage( me->speed );
	config_set_fan_state( me->currentState );
}

/* -------------------------------------------------------------------------- */

// Calculate an appropriate fan speed percentage based on temperature lookup table.

PRIVATE uint8_t
fan_speed_at_temp( float temperature )
{
	//protect against out-of-bounds temperature inputs
    if( temperature < fan_curve[ 0 ].temp )
    {
    	//temp is lower than lowest point in LUT
        return 0.0;
    }
    else if( temperature > fan_curve[ DIM(fan_curve) - 1 ].temp )
    {
    	//temp exceeds max LUT value
        return 100.0;
    }

    for( uint8_t i = 0; i < DIM(fan_curve) - 1; i++ )
    {
    	//within range between two rows of the LUT
        if( temperature > fan_curve[ i ].temp && temperature <= fan_curve[ i + 1 ].temp )
        {
        	//linear interpolation for fan speed between the surrounding rows in LUT
        	return fan_curve[ i ].percentage + ( ((temperature - fan_curve[ i ].temp)/(fan_curve[ i + 1 ].temp - fan_curve[ i ].temp)) * ( fan_curve[ i + 1 ].percentage - fan_curve[ i ].percentage ) ) ;
        }
    }

    return 97;	// should have returned with a valid percentage by now, fail ON for safety.
}

/* ----- End ---------------------------------------------------------------- */
