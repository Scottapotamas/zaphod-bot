/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "clearpath.h"
#include "sensors.h"

#include "hal_systick.h"
#include "hal_delay.h"
#include "hal_gpio.h"
#include "hal_hard_ic.h"

#include "global.h"
#include "qassert.h"
#include "simple_state_machine.h"
#include "app_times.h"
#include "event_subscribe.h"
#include "app_signals.h"
#include "configuration.h"
#include "status.h"

/* ----- Defines ------------------------------------------------------------ */

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
	bool			feedback_ok;

	int16_t		angle_current_steps;
	int16_t		angle_target_steps;
} Servo_t;

typedef struct
{
	//Servo IO
	HalGpioPortPin_t 	pin_enable;
	HalGpioPortPin_t 	pin_direction;
	HalGpioPortPin_t 	pin_step;
    HalGpioPortPin_t 	pin_feedback;

    InputCaptureSignal_t 	ic_feedback;

	//Current Sense IC
	HalAdcInput_t		adc_current;
	HalGpioPortPin_t	pin_oc_fault;

} ServoHardware_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Servo_t clearpath[ _NUMBER_CLEARPATH_SERVOS ];

PRIVATE const ServoHardware_t ServoHardwareMap[] =
{
		[ _CLEARPATH_1 ]	= {     .pin_enable = _SERVO_1_ENABLE,
                                    .pin_direction = _SERVO_1_A,
                                    .pin_step = _SERVO_1_B,
                                    .pin_feedback = _SERVO_1_HLFB,
                                    .ic_feedback = HAL_HARD_IC_HLFB_SERVO_1,
                                    .adc_current = HAL_ADC_INPUT_M1_CURRENT,
                                    .pin_oc_fault = _SERVO_1_CURRENT_FAULT },

		[ _CLEARPATH_2 ]	= {     .pin_enable = _SERVO_2_ENABLE,
                                    .pin_direction = _SERVO_2_A,
                                    .pin_step = _SERVO_2_B,
                                    .pin_feedback = _SERVO_2_HLFB,
                                    .ic_feedback = HAL_HARD_IC_HLFB_SERVO_2,
                                    .adc_current = HAL_ADC_INPUT_M2_CURRENT,
                                    .pin_oc_fault = _SERVO_2_CURRENT_FAULT },

		[ _CLEARPATH_3 ]	= {     .pin_enable = _SERVO_3_ENABLE,
                                    .pin_direction = _SERVO_3_A,
                                    .pin_step = _SERVO_3_B,
                                    .pin_feedback = _SERVO_3_HLFB,
                                    .ic_feedback = HAL_HARD_IC_HLFB_SERVO_3,
                                    .adc_current = HAL_ADC_INPUT_M3_CURRENT,
                                    .pin_oc_fault = _SERVO_3_CURRENT_FAULT },

#ifdef EXPANSION_SERVO
        [ _CLEARPATH_4 ]	= { .pin_enable = _SERVO_4_ENABLE,
                                .pin_direction = _SERVO_4_A,
                                .pin_step = _SERVO_4_B,
                                .pin_feedback = _SERVO_4_HLFB,
                                .ic_feedback = HAL_HARD_IC_HLFB_SERVO_4,
                                .adc_current = HAL_ADC_INPUT_M4_CURRENT,
                                .pin_oc_fault = _SERVO_4_CURRENT_FAULT },
#endif
};

PRIVATE float       servo_get_hlfb_percent( ClearpathServoInstance_t servo );

PRIVATE int16_t 	convert_angle_steps( float kinematics_shoulder_angle );

PRIVATE float 		convert_steps_angle( int16_t steps );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
servo_init( ClearpathServoInstance_t servo )
{
	memset( &clearpath[servo], 0, sizeof( Servo_t ) );

}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_start( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];
    me->enabled = SERVO_ENABLE;
    me->home_complete = 0;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_stop( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];
    me->enabled = SERVO_DISABLE;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_set_target_angle( ClearpathServoInstance_t servo, float angle_degrees )
{
    Servo_t *me = &clearpath[servo];

    config_motor_target_angle( servo, angle_degrees);

    if( angle_degrees > (SERVO_MIN_ANGLE * -1) && angle_degrees < SERVO_MAX_ANGLE)
    {
        me->angle_target_steps = convert_angle_steps( angle_degrees );
    }

}

/* -------------------------------------------------------------------------- */

PUBLIC float
servo_get_current_angle( ClearpathServoInstance_t servo)
{
	Servo_t *me = &clearpath[servo];

	return convert_steps_angle( me->angle_current_steps );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_get_move_done( ClearpathServoInstance_t servo)
{
	Servo_t *me = &clearpath[servo];

	return ( me->angle_current_steps == me->angle_target_steps );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_get_servo_ok(ClearpathServoInstance_t servo)
{
	Servo_t *me = &clearpath[servo];

	return ( me->enabled && me->feedback_ok );
}

/* -------------------------------------------------------------------------- */

float mapf(float val, float in_min, float in_max, float out_min, float out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// HLFB from servos is a 482Hz square-wave, where 5% < x < 95% is used for torque/speed output
// DutyCycles under 5% represents 'not asserted' or 'off'

PRIVATE float
servo_get_hlfb_percent( ClearpathServoInstance_t servo )
{
    // InputCapture value is a integer representing the 'on' duration of the wave in us

    // 0.055679% per count
    // min of 62 at 4.0%
    // max of 2041 at 97.0%

    float percentage = 0.0f;

    // If there is a valid IC value, grab the average IC duration and convert to a percentage
    if( hal_hard_ic_is_recent(ServoHardwareMap[servo].ic_feedback) )
    {
        percentage = mapf( hal_hard_ic_read(ServoHardwareMap[servo].ic_feedback), 62, 2041, 4.0f,97.0f );
        CLAMP(percentage, 5.0f, 95.0f);
    }
    else
    {
        // IC hasn't fired recently, so the signal is likely DC LOW or HIGH
        percentage = (hal_gpio_read_pin(ServoHardwareMap[servo].pin_feedback))? 100.0f: 0.0f;
    }

    return percentage;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_process( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];

    float servo_power = sensors_servo_W( ServoHardwareMap[servo].adc_current );
    float servo_feedback = servo_get_hlfb_percent(servo);
    me->feedback_ok = ( servo_feedback > 5.0f );

    switch( me->currentState )
    {
        case SERVO_STATE_INACTIVE:
            STATE_ENTRY_ACTION
				hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, 	SERVO_DISABLE );
				hal_gpio_write_pin( ServoHardwareMap[servo].pin_step, 		false );
				hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, 	false );
				me->enabled = SERVO_DISABLE;

            STATE_TRANSITION_TEST

				if( me->enabled )
				{
					STATE_NEXT( SERVO_STATE_HOMING );
				}

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_ERROR_RECOVERY:
            STATE_ENTRY_ACTION

				hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, 	SERVO_DISABLE );
				hal_gpio_write_pin( ServoHardwareMap[servo].pin_step, 		false );
				hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, 	false );

				me->enabled = SERVO_DISABLE;
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
				//expect the motor to slowly home, stop, move to the neutral point, then stop
				//guard times for min and max homing delays (how long the motor would take in worst case to home
            	if( me->feedback_ok )
				{
					//motor homed too fast (it needs to transit to the endstop, then back to the 'neutral' angle), which incurs travel time
					if( ( hal_systick_get_ms() - me->timer ) < SERVO_HOMING_MIN_MS )
					{
						//it is possible that the servo position was already resting on/near the endstop, so a (singular) OK state is allowed and flagged
						if( me->home_complete >= SERVO_HOMING_NULL_PERIODS_ALLOWED )
						{
							//something is wrong
						    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
						}
						else
						{
							//we think that null period is a valid part of the homing sequence, lets see how the rest of the sequence goes
							me->home_complete++;
						}
					}
					else
					{
						//motor stopped moving after the minimum time period, which indicates a successful home procedure
						me->enabled = SERVO_ENABLE;
						me->angle_current_steps = convert_angle_steps(-42.0f);
                        config_motor_target_angle( servo, -42.0f);  // update UI have angles before a target is sent in

                        me->angle_target_steps = me->angle_current_steps;

                        STATE_NEXT( SERVO_STATE_IDLE );
					}
				}

				//motor is taking too long to complete the homing process
				if( ( hal_systick_get_ms() - me->timer ) > SERVO_HOMING_MAX_MS )
				{
				    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
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
	            	//check if the motor has been drawing higher than expected power while stationary
	            	if( servo_power > SERVO_IDLE_POWER_ALERT_W )
	            	{
	            		//something might be wrong, watch it more closely
						STATE_NEXT( SERVO_STATE_IDLE_HIGH_LOAD );
	            	}
				}

				if( !me->enabled )
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

            	//read the smoothed power draw, we expect some load due to end effector or forces imparted from extra-axis movements
				if( servo_power > SERVO_IDLE_POWER_ALERT_W )
				{
					//been measuring a pretty high load for a while now
					if( ( hal_systick_get_ms() - me->timer ) > SERVO_IDLE_POWER_TRIP_MS )
					{
						//shutdown for safety
						config_report_error( "Servo Overload");
                        eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
                        STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
					}
				}
				else
				{
					//if we've been under the alert threshold for a while, return to the idle state
					if( ( hal_systick_get_ms() - me->timer ) > SERVO_IDLE_POWER_TRIP_MS*2 )
					{
						STATE_NEXT( SERVO_STATE_IDLE );
					}
				}

				//been disabled or current sensor has flagged a fault, shutdown
				if( !me->enabled || hal_gpio_read_pin( ServoHardwareMap[servo].pin_oc_fault ) == SERVO_OC_FAULT )
				{
					STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
				}
            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_ACTIVE:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST
                int16_t step_difference = me->angle_current_steps - me->angle_target_steps;
                int8_t  step_direction = 0;

                //command rotation to move towards the target position
                if( step_difference != 0 )
                {
                    //command the target direction
                    if( me->angle_current_steps < me->angle_target_steps )
                    {
                        hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, SERVO_DIR_CW );
                        step_direction = -1;
                    }
                    else
                    {
                        hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, SERVO_DIR_CCW );
                        step_direction = 1;
                    }

                    uint16_t pulses_needed = step_difference*step_direction;


                    if( pulses_needed > 5 )
                    {
                        pulses_needed = 5;
                        status_yellow(true);    // visual debugging aid to see when speed limits are hit
                    }
                    else
                    {
                        status_yellow(false);
                    }

                    for( uint16_t pulses = 0; pulses < pulses_needed; pulses++ )
                    {
                        hal_gpio_toggle_pin( ServoHardwareMap[servo].pin_step );
                        hal_delay_us( SERVO_PULSE_DURATION_US );
                        hal_gpio_toggle_pin( ServoHardwareMap[servo].pin_step );
                        hal_delay_us( SERVO_PULSE_DURATION_US );
                        me->angle_current_steps = me->angle_current_steps + (step_direction*-1);
                    }

                }
                else
                {
                    STATE_NEXT( SERVO_STATE_IDLE );
                }

                if( !me->enabled || hal_gpio_read_pin( ServoHardwareMap[servo].pin_oc_fault ) == SERVO_OC_FAULT )
                {
                    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                }
            STATE_EXIT_ACTION

            STATE_END
            break;
    }

    config_motor_state( servo , me->currentState);
    config_motor_enable( servo, me->enabled );
    config_motor_feedback( servo, servo_feedback );
    config_motor_power( servo, servo_power);

}

/* -------------------------------------------------------------------------- */

/*
 * The kinematics output is an angle between -85 and +90, where 0 deg is when
 * the elbow-shaft link is parallel to the frame plate. Full range not available due
 * to physical limits on arm travel. Approx -45 to +70 is the safe working range.
 *
 * Servo steps are referenced to the homing point, which is the HW minimum,
 * therefore we need to convert the angle into steps, and apply the adequate offset.
 * The servo's range is approx 2300 counts.
 */
PRIVATE int16_t
convert_angle_steps( float kinematics_shoulder_angle )
{
	float converted_angle = kinematics_shoulder_angle + SERVO_MIN_ANGLE;
	int16_t angle_as_steps = converted_angle * SERVO_STEPS_PER_DEGREE;

	return angle_as_steps;
}

/* -------------------------------------------------------------------------- */

/*
 * Convert a motor position in steps to an angle in the motor reference frame (not kinematics shoulder angle)
 */
PRIVATE float
convert_steps_angle( int16_t steps )
{
    float steps_to_angle = (float)steps / SERVO_STEPS_PER_DEGREE;
	return steps_to_angle;
}
/* ----- End ---------------------------------------------------------------- */
