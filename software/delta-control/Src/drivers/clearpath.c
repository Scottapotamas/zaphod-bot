/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "clearpath.h"
#include "hal_systick.h"
#include "hal_delay.h"
#include "hal_gpio.h"
#include "hal_adc.h"
#include "hal_pwm.h"
#include "hal_power.h"

#include "global.h"
#include "qassert.h"
#include "simple_state_machine.h"
#include "app_times.h"


/* ----- Defines ------------------------------------------------------------ */

/* ~~~ State Machine Handling Macros ~~~ */


/* ~~~ State Machine Variables ~~~ */


#define SERVO_STEPS_PER_REV					6400
#define SERVO_ANGLE_PER_REV 				360
#define SERVO_MIN_ANGLE 					10
#define SERVO_MAX_ANGLE 					20
#define SERVO_SOFT_LIMIT_DEPTH_STEPS		10

//Homing parameters
#define SERVO_INTERRUPTED_DISABLE_DELAY_MIN_MS 50
#define SERVO_HOMING_MIN_MS 				100
#define SERVO_HOMING_MAX_MS 				5000
#define SERVO_HOMING_NULL_PERIODS_ALLOWED 	2

//Error evaluation parameters
#define SERVO_IDLE_CURRENT_ALERT_A 			1.0f	//~70W
#define SERVO_PEAK_CURRENT_A 				2.85f	//~220W
#define SERVO_IDLE_SETTLE_MS				70
#define SERVO_IDLE_CURRENT_TRIP_MS			400

#define SERVO_OC_FAULT	false
#define SERVO_OC_OK		true

//Fault handling
#define SERVO_FAULT_LINGER_MS 				8000


//Clearpath will filter pulses shorter than 1us
//ULN2303 NPN driver has rise time of ~5ns, fall of ~10nsec
#define SERVO_PULSE_DURATION_US 2


//Clearpath input high = clockwise rotation. Alias direction against pin state
#define SERVO_DIR_CCW	true
#define SERVO_DIR_CW	false

//Clearpath EN high = enable/GO
#define SERVO_ENABLE	true
#define SERVO_DISABLE	false


typedef enum
{
    SERVO_STATE_INACTIVE,
	SERVO_STATE_ERROR_RECOVERY,
	SERVO_STATE_HOMING,
	SERVO_STATE_IDLE,
	SERVO_STATE_IDLE_HIGH_LOAD,
	SERVO_STATE_ACTIVE,
} ServoState_t;

typedef struct
{
	ServoState_t   	previousState;
	ServoState_t   	currentState;
	ServoState_t   	nextState;

	bool			enabled;
	uint8_t			home_complete;
	uint32_t      	timer;

	uint16_t		angle_current_steps;
	uint16_t		angle_target_steps;
} Servo_t;

typedef struct
{
	//Servo IO
	HalGpioPortPin_t 	pin_enable;
	HalGpioPortPin_t 	pin_direction;
	HalGpioPortPin_t 	pin_step;
	HalGpioPortPin_t 	pin_feedback;
	HLFBTimerDef_t		pwm_feedback;

	//Current Sense IC
	HalAdcInput_t		adc_current;
	HalGpioPortPin_t	pin_oc_fault;

} ServoHardware_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Servo_t clearpath[ _NUMBER_CLEARPATH_SERVOS ];

PRIVATE const ServoHardware_t ServoHardwareMap[] =
{
		[ _CLEARPATH_1 ]	= { .pin_enable = _SERVO_1_ENABLE, .pin_direction = _SERVO_1_A, .pin_step = _SERVO_1_B, .pin_feedback = _SERVO_1_HLFB, .pwm_feedback = _HLFB_SERVO_1,
								.adc_current = HAL_ADC_INPUT_M1_CURRENT, .pin_oc_fault = _SERVO_1_CURRENT_FAULT },

		[ _CLEARPATH_2 ]	= { .pin_enable = _SERVO_2_ENABLE, .pin_direction = _SERVO_2_A, .pin_step = _SERVO_2_B, .pin_feedback = _SERVO_2_HLFB, .pwm_feedback = _HLFB_SERVO_2,
								.adc_current = HAL_ADC_INPUT_M2_CURRENT, .pin_oc_fault = _SERVO_2_CURRENT_FAULT },

		[ _CLEARPATH_3 ]	= { .pin_enable = _SERVO_3_ENABLE, .pin_direction = _SERVO_3_A, .pin_step = _SERVO_3_B, .pin_feedback = _SERVO_3_HLFB, .pwm_feedback = _HLFB_SERVO_3,
								.adc_current = HAL_ADC_INPUT_M3_CURRENT, .pin_oc_fault = _SERVO_3_CURRENT_FAULT },

		[ _CLEARPATH_4 ]	= { .pin_enable = _SERVO_4_ENABLE, .pin_direction = _SERVO_4_A, .pin_step = _SERVO_4_B, .pin_feedback = _SERVO_4_HLFB, .pwm_feedback = _HLFB_SERVO_4,
								.adc_current = HAL_ADC_INPUT_M4_CURRENT, .pin_oc_fault = _SERVO_4_CURRENT_FAULT },
};

PRIVATE uint16_t convert_angle_steps(float shaft_angle);

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
servo_init( ClearpathServoInstance_t servo )
{
	memset( &clearpath[servo], 0, sizeof( Servo_t ) );

	//init the hlfb input capture?

}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_run_startup( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];

    //consider the motor was already running or homed
    //disable the motor, wait out a grace period, then resume the normal homing process
    if( me->enabled || me->home_complete )
    {
    	hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, SERVO_DISABLE );
    	me->enabled = false;
    	hal_delay_ms( SERVO_INTERRUPTED_DISABLE_DELAY_MIN_MS );
    }

    STATE_NEXT( SERVO_STATE_HOMING );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_set_target_angle( ClearpathServoInstance_t servo, float angle_degrees )
{
    Servo_t *me = &clearpath[servo];

    me->angle_target_steps = convert_angle_steps( angle_degrees );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_process( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];

    switch( me->currentState )
    {
        case SERVO_STATE_INACTIVE:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_ERROR_RECOVERY:
            STATE_ENTRY_ACTION

				hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, 	SERVO_DISABLE );
				hal_gpio_write_pin( ServoHardwareMap[servo].pin_step, 		false );
				hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, 	false );

				me->timer = hal_systick_get_ms();

            STATE_TRANSITION_TEST

				if( ( hal_systick_get_ms() - me->timer ) > SERVO_FAULT_LINGER_MS )
				{
				    STATE_NEXT( SERVO_STATE_INACTIVE );
				}

            	//todo ensure no motion on mechanism for some period of time

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING:
            STATE_ENTRY_ACTION

				//EN pin high starts homing process on clearpath servo
				//servo homing behaviour is defined with the clearpath setup software, uses torque based end-stop sensing
				hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, SERVO_ENABLE );
        		me->home_complete = 0;

            	me->timer = hal_systick_get_ms();

            STATE_TRANSITION_TEST

				//the HLFB output pin will be set to speed mode, outputs 45Hz PWM where DC% relates to the velocity.
				//expect the motor to slowly home, stop, move to the neutral point, then stop
				//guard times for min and max homing delays (how long the motor would take in worst case to home
				bool status_ok = 1;	//todo read hlfb signal here

				if( status_ok )
				{
					//motor homed too fast (it needs to transit to the endstop, then back to the 'neutral' angle, which incurs travel time
					if( ( hal_systick_get_ms() - me->timer ) < SERVO_HOMING_MIN_MS )
					{
						//it is possible that the servo position was already resting on/near the endstop, so a (singular) ok state is allowed and flagged
						if( me->home_complete >= SERVO_HOMING_NULL_PERIODS_ALLOWED )
						{
							//something is wrong
						    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
							//todo flag error
						}
						else
						{
							//we think that null period is a valid part of the homing sequence, lets see how the rest of the sequence goes
							me->home_complete++;
						}
					}
					else
					{
						//motor stopped moving after the minimum time period, which indicates a successful home proceedure
						me->enabled = SERVO_ENABLE;
					    STATE_NEXT( SERVO_STATE_IDLE );
					}
				}

				//motor is taking too long to complete the homing process
				if( ( hal_systick_get_ms() - me->timer ) > SERVO_HOMING_MAX_MS )
				{
				    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
					//todo flag error
				}

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_IDLE:
            STATE_ENTRY_ACTION

    			me->timer = hal_systick_get_ms();

            STATE_TRANSITION_TEST

				if( me->angle_current_steps != me->angle_target_steps)
				{
					STATE_NEXT( SERVO_STATE_ACTIVE );
				}

            	//allow some time for the motor to decelerate before we worry about excessive no-motion loads
				if( hal_systick_get_ms() - me->timer > SERVO_IDLE_SETTLE_MS )
				{
	            	//check if the motor has been drawing higher than expected current while stationary
	            	if( hal_adc_read_peak( ServoHardwareMap[servo].adc_current) > SERVO_IDLE_CURRENT_ALERT_A )
	            	{
	            		//something might be wrong, watch it more closely
						STATE_NEXT( SERVO_STATE_IDLE_HIGH_LOAD );
	            	}
				}

				//current sensor has flagged a fault
				if( hal_gpio_read_pin( ServoHardwareMap[servo].pin_oc_fault ) == SERVO_OC_FAULT )
				{
					STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
				}

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_IDLE_HIGH_LOAD:
            STATE_ENTRY_ACTION

				me->timer = hal_systick_get_ms();

            STATE_TRANSITION_TEST

				if( me->angle_current_steps != me->angle_target_steps)
				{
					STATE_NEXT( SERVO_STATE_ACTIVE );
				}

            	//read the smoothed current draw, we expect some load due to end effector or forces imparted from extra-axis movements
				if( hal_adc_read_avg( ServoHardwareMap[servo].adc_current) > SERVO_IDLE_CURRENT_ALERT_A )
				{
					//been measuring a pretty high load for a while now
					if( ( hal_systick_get_ms() - me->timer ) > SERVO_IDLE_CURRENT_TRIP_MS )
					{
						//shutdown for safety
						STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
					}
				}
				else
				{
					//if we've been under the alert threshold for a while, return to the idle state
					if( ( hal_systick_get_ms() - me->timer ) > SERVO_IDLE_CURRENT_TRIP_MS )
					{
						STATE_NEXT( SERVO_STATE_IDLE );
					}
				}

				//current sensor has flagged a fault
				if( hal_gpio_read_pin( ServoHardwareMap[servo].pin_oc_fault ) == SERVO_OC_FAULT )
				{
					STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
				}

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_ACTIVE:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST

				//command the servo's target direction
				if( me->angle_current_steps > me->angle_target_steps )
				{
					hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, SERVO_DIR_CCW );
				}
				else
				{
					hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, SERVO_DIR_CW );
				}

            	//command rotation to move towards the target position
				if( me->angle_current_steps != me->angle_target_steps )
				{
					//todo add 'multi-pulse' control with acceleration shaping?

					hal_gpio_toggle_pin( ServoHardwareMap[servo].pin_step );
					hal_delay_us( SERVO_PULSE_DURATION_US );
					hal_gpio_toggle_pin( ServoHardwareMap[servo].pin_step );
				}
				else
				{
					STATE_NEXT( SERVO_STATE_IDLE );
				}

				//current sensor has flagged a fault
				if( hal_gpio_read_pin( ServoHardwareMap[servo].pin_oc_fault ) == SERVO_OC_FAULT )
				{
					STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
				}

            STATE_EXIT_ACTION

            STATE_END
            break;
    }
}

/* -------------------------------------------------------------------------- */

PRIVATE uint16_t
convert_angle_steps(float shaft_angle)
{
	return shaft_angle * SERVO_STEPS_PER_REV;
}

/* ----- End ---------------------------------------------------------------- */
