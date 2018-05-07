/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "fan.h"
#include "simple_state_machine.h"
#include "hal_pwm.h"
#include "hal_systick.h"

/* ----- Private Types ------------------------------------------------------ */

typedef enum
{
    FAN_STATE_OFF,
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

#define FAN_STARTUP_TIME_MS 1000
#define FAN_STALL_FAULT_RPM 20
#define FAN_FREQUENCY_HZ 25000UL

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
fan_init( void )
{
    memset( &fan, 0, sizeof( fan ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_set( uint8_t speed_percentage )
{
	Fan_t *me = &fan;

	//0-100% speed control over fan
	me->set_speed = MAX( MIN(speed_percentage, 100), 0);

    //STATE_INIT_INITIAL( FAN_STATE_ON );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_process( void )
{
	Fan_t *me = &fan;

    switch( me->currentState )
    {
        case FAN_STATE_OFF:
            STATE_ENTRY_ACTION

				//make sure fan is not spinning?

            STATE_TRANSITION_TEST

				//once new target is established, trigger startup blip
                if( me->set_speed > 0 )
                {
                    STATE_NEXT( FAN_STATE_START );
                }

            STATE_EXIT_ACTION
            STATE_END
            break;

        case FAN_STATE_START:
            STATE_ENTRY_ACTION

				//set PWM to 100% for configurable short period
            	hal_pwm_setup_output( _PWM_TIM_FAN, FAN_FREQUENCY_HZ, 100 );

            	me->startup_timer = hal_systick_get_ms();
            	me->speed = 100;

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

				hal_pwm_setup_output( _PWM_TIM_FAN, FAN_FREQUENCY_HZ, me->set_speed );
				me->speed = me->set_speed;

            STATE_TRANSITION_TEST

				//speed change req while running
				if( me->set_speed != me->speed )
				{
					hal_pwm_setup_output( _PWM_TIM_FAN, FAN_FREQUENCY_HZ, me->set_speed );
					me->speed = me->set_speed;
				}

            	//fan req turned off
            	if( me->set_speed == 0 )
                {
                    STATE_NEXT( FAN_STATE_OFF );
                }

            	//rotor stop detection
            	uint16_t fan_hall_rpm = 200;	//fake hall sensor value

            	if( fan_hall_rpm < FAN_STALL_FAULT_RPM )
            	{
            		//set target speed to be slightly higher than the current speed?

            		//restart the fan
                    STATE_NEXT( FAN_STATE_START );
            	}

            STATE_EXIT_ACTION

            	hal_pwm_setup_output( _PWM_TIM_FAN, 0, 0 );

            STATE_END
            break;
    }
}

/* ----- End ---------------------------------------------------------------- */
