/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "clearpath.h"
#include "sensors.h"

#include "hal_delay.h"
#include "hal_gpio.h"
#include "hal_hard_ic.h"
#include "hal_systick.h"

#include "app_signals.h"
#include "app_times.h"
#include "configuration.h"
#include "event_subscribe.h"
#include "global.h"
#include "qassert.h"
#include "simple_state_machine.h"
#include "status.h"

/* ----- Defines ------------------------------------------------------------ */

typedef enum
{
    SERVO_STATE_INACTIVE,
    SERVO_STATE_ERROR_RECOVERY,

    SERVO_STATE_HOMING_CALIBRATE_TORQUE,
    SERVO_STATE_HOMING_FIND_ENDSTOP,
    SERVO_STATE_HOMING_FOUND_ENDSTOP,
    SERVO_STATE_HOMING_CHECK_FOLDBACK,
    SERVO_STATE_HOMING_SUCCESS,

    SERVO_STATE_IDLE,
    SERVO_STATE_IDLE_HIGH_LOAD,
    SERVO_STATE_ACTIVE,
} ServoState_t;

typedef struct
{
    ServoState_t previousState;
    ServoState_t currentState;
    ServoState_t nextState;
    uint32_t     timer;

    float   ic_feedback_trim;
    float   homing_feedback;
    int16_t angle_current_steps;
    int16_t angle_target_steps;
    bool    enabled;
} Servo_t;

typedef struct
{
    // Servo IO
    HalGpioPortPin_t pin_enable;
    HalGpioPortPin_t pin_direction;
    HalGpioPortPin_t pin_step;
    HalGpioPortPin_t pin_feedback;

    // HLFB (HighLevelFeedBack) from servo
    InputCaptureSignal_t ic_feedback;

    // Current Sense IC
    HalAdcInput_t    adc_current;
    HalGpioPortPin_t pin_oc_fault;

} ServoHardware_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Servo_t clearpath[_NUMBER_CLEARPATH_SERVOS];

PRIVATE const ServoHardware_t ServoHardwareMap[] = {
    [_CLEARPATH_1] = { .pin_enable    = _SERVO_1_ENABLE,
                       .pin_direction = _SERVO_1_A,
                       .pin_step      = _SERVO_1_B,
                       .pin_feedback  = _SERVO_1_HLFB,
                       .ic_feedback   = HAL_HARD_IC_HLFB_SERVO_1,
                       .adc_current   = HAL_ADC_INPUT_M1_CURRENT,
                       .pin_oc_fault  = _SERVO_1_CURRENT_FAULT },

    [_CLEARPATH_2] = { .pin_enable    = _SERVO_2_ENABLE,
                       .pin_direction = _SERVO_2_A,
                       .pin_step      = _SERVO_2_B,
                       .pin_feedback  = _SERVO_2_HLFB,
                       .ic_feedback   = HAL_HARD_IC_HLFB_SERVO_2,
                       .adc_current   = HAL_ADC_INPUT_M2_CURRENT,
                       .pin_oc_fault  = _SERVO_2_CURRENT_FAULT },

    [_CLEARPATH_3] = { .pin_enable    = _SERVO_3_ENABLE,
                       .pin_direction = _SERVO_3_A,
                       .pin_step      = _SERVO_3_B,
                       .pin_feedback  = _SERVO_3_HLFB,
                       .ic_feedback   = HAL_HARD_IC_HLFB_SERVO_3,
                       .adc_current   = HAL_ADC_INPUT_M3_CURRENT,
                       .pin_oc_fault  = _SERVO_3_CURRENT_FAULT },

#ifdef EXPANSION_SERVO
    [_CLEARPATH_4] = { .pin_enable    = _SERVO_4_ENABLE,
                       .pin_direction = _SERVO_4_A,
                       .pin_step      = _SERVO_4_B,
                       .pin_feedback  = _SERVO_4_HLFB,
                       .ic_feedback   = HAL_HARD_IC_HLFB_SERVO_4,
                       .adc_current   = HAL_ADC_INPUT_M4_CURRENT,
                       .pin_oc_fault  = _SERVO_4_CURRENT_FAULT },
#endif
};

PRIVATE float servo_get_hlfb_percent( ClearpathServoInstance_t servo );

PRIVATE float servo_get_hlfb_percent_corrected( ClearpathServoInstance_t servo );

PRIVATE int16_t convert_angle_steps( float kinematics_shoulder_angle );

PRIVATE float convert_steps_angle( int16_t steps );

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

    config_motor_target_angle( servo, angle_degrees );

    if( angle_degrees > ( SERVO_MIN_ANGLE * -1 ) && angle_degrees < SERVO_MAX_ANGLE )
    {
        me->angle_target_steps = convert_angle_steps( angle_degrees );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC float
servo_get_current_angle( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];

    return convert_steps_angle( me->angle_current_steps );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_get_move_done( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];

    return ( me->angle_current_steps == me->angle_target_steps );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_get_servo_ok( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];

    return ( me->enabled
             && ( me->currentState == SERVO_STATE_IDLE || me->currentState == SERVO_STATE_IDLE_HIGH_LOAD || me->currentState == SERVO_STATE_ACTIVE ) );
}

PUBLIC bool
servo_get_servo_did_error( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];

    return ( me->currentState == SERVO_STATE_ERROR_RECOVERY || me->previousState == SERVO_STATE_ERROR_RECOVERY || me->nextState == SERVO_STATE_ERROR_RECOVERY );
}

/* -------------------------------------------------------------------------- */

// Returns uncorrected servo feedback torque as a percentage from -100% to 100% of rated capability
PRIVATE float
servo_get_hlfb_percent( ClearpathServoInstance_t servo )
{

    float percentage = 0.0f;

    // HLFB from servos is a square-wave where 5% < x < 95% is used for torque/speed output
    // The hardware input capture driver returns us the value as %duty cycle
    // 65% DC => 1/3rd max torque in +ve direction

    // Get the HLFB duty, and scale to -100% to +100% range
    percentage = hal_hard_ic_read_f( ServoHardwareMap[servo].ic_feedback ) * 2.05 - 100.0;
    CLAMP( percentage, -100.0f, 100.0f );

    return percentage;
}

// Corrected servo feedback uses a trim value calculated during the arming procedure
PRIVATE float
servo_get_hlfb_percent_corrected( ClearpathServoInstance_t servo )
{
    return servo_get_hlfb_percent( servo ) - clearpath[servo].ic_feedback_trim;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_process( ClearpathServoInstance_t servo )
{
    Servo_t *me = &clearpath[servo];

    float servo_power    = sensors_servo_W( ServoHardwareMap[servo].adc_current );
    float servo_feedback = servo_get_hlfb_percent_corrected( servo );

    switch( me->currentState )
    {
        case SERVO_STATE_INACTIVE:
            STATE_ENTRY_ACTION
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, SERVO_DISABLE );
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_step, false );
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, false );
            me->enabled = SERVO_DISABLE;

            STATE_TRANSITION_TEST

            if( me->enabled )
            {
                STATE_NEXT( SERVO_STATE_HOMING_CALIBRATE_TORQUE );
            }

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_ERROR_RECOVERY:
            STATE_ENTRY_ACTION

            hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, SERVO_DISABLE );
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_step, false );
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, false );

            me->enabled = SERVO_DISABLE;
            me->timer   = hal_systick_get_ms();

            STATE_TRANSITION_TEST

            if( ( hal_systick_get_ms() - me->timer ) > SERVO_FAULT_LINGER_MS )
            {
                STATE_NEXT( SERVO_STATE_INACTIVE );
            }

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING_CALIBRATE_TORQUE:
            STATE_ENTRY_ACTION
            clearpath[servo].ic_feedback_trim = 0.0f;
            me->timer                         = hal_systick_get_ms();
            STATE_TRANSITION_TEST
            float uncorrected_feedback = servo_get_hlfb_percent( servo );

            if( ( hal_systick_get_ms() - me->timer ) < SERVO_HOMING_CALIBRATION_MS )
            {
                // The trim value is a super simple average of the incoming values
                clearpath[servo].ic_feedback_trim = ( 0.1f * uncorrected_feedback ) + ( clearpath[servo].ic_feedback_trim * 0.9f );
            }
            else
            {
                // Check that the corrected feedback value with our trim is pretty close to zero
                if( -0.5f < servo_feedback && servo_feedback < 0.5f )
                {
                    STATE_NEXT( SERVO_STATE_HOMING_FIND_ENDSTOP );
                    uncorrected_feedback = 0.0f;
                }
                else
                {
                    // An average of the disabled motor's feedback somehow didn't offset an error
                    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                }
            }

            // Detecting high torque values during calibration sounds like a hardware fault
            if( uncorrected_feedback < -1 * SERVO_HOMING_CALIBRATION_TORQUE || SERVO_HOMING_CALIBRATION_TORQUE < uncorrected_feedback )
            {
                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
            }
            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING_FIND_ENDSTOP:
            STATE_ENTRY_ACTION
            // EN pin high starts automatic homing process on clearpath servo
            // Servo homing behaviour is defined with the clearpath setup software, uses torque based end-stop sensing
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, SERVO_ENABLE );
            me->timer = hal_systick_get_ms();
            STATE_TRANSITION_TEST
            // We expect the torque to be positive while the arm transits towards the endstop
            // When it goes under 0% HLFB, the torque has changed direction and we've likely just contacted the endstop
            // TODO consider how this homing behaviour works when delta is suspended?
            if( servo_feedback < -1.0f )
            {
                // After this point, we'd expect the torque to increase (in -ve direction) until foldback kicks in
                STATE_NEXT( SERVO_STATE_HOMING_FOUND_ENDSTOP );
            }

            // TODO timeout if we don't transit there in some reasonable amount of time?
            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING_FOUND_ENDSTOP:
            STATE_ENTRY_ACTION
            me->timer           = hal_systick_get_ms();
            me->homing_feedback = servo_feedback;
            STATE_TRANSITION_TEST
            // We think the servo has found the endstop. Torque should be increasing until servo hits the homing threshold.
            // Once the servo reaches its thresholded homing torque, torque foldback kicks in
            if( servo_feedback < me->homing_feedback )
            {
                //increasing torque in -ve direction
                me->homing_feedback = servo_feedback;
            }
            else
            {
                // Found the a local peak, check it's over the servo's expected threshold + some error padding
                if( servo_feedback < -1 * SERVO_HOMING_ENDSTOP_RAMP_MIN )
                {
                    STATE_NEXT( SERVO_STATE_HOMING_CHECK_FOLDBACK );
                }
            }

            // Error if the homing force is higher than expected
            if( servo_feedback < ( -1 * SERVO_HOMING_ENDSTOP_RAMP_MAX ) )
            {
                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
            }
            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING_CHECK_FOLDBACK:
            STATE_ENTRY_ACTION
            me->timer = hal_systick_get_ms();

            STATE_TRANSITION_TEST
            // Check that the servo is loaded against the endstop in foldback mode
            // When we enter this state, foldback is likely still heading to setpoint
            // Check the value is reasonable for a continuous slice of time
            if( ( hal_systick_get_ms() - me->timer ) > SERVO_HOMING_FOLDBACK_CHECK_START_MS )
            {
                // Check if foldback holding torque value is outside of expected range...
                if( servo_feedback > -1 * SERVO_HOMING_FOLDBACK_TORQUE_MIN
                    && servo_feedback < -1 * SERVO_HOMING_FOLDBACK_TORQUE_MAX )
                {

                    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                }

                // If we made it this far while being in range, everything looks good
                if( ( hal_systick_get_ms() - me->timer ) < SERVO_HOMING_FOLDBACK_CHECK_END_MS )
                {
                    STATE_NEXT( SERVO_STATE_HOMING_SUCCESS );
                }
            }
            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING_SUCCESS:
            STATE_ENTRY_ACTION
            me->timer           = hal_systick_get_ms();
            me->homing_feedback = 0.0f;
            STATE_TRANSITION_TEST
            // Servo successfully found home, and is likely moving to the start-offset point
            // Wait for the servo to settle in torque domain
            float feedback_error = ( servo_feedback - me->homing_feedback );
            me->homing_feedback  = ( 0.1f * servo_feedback ) + ( clearpath[servo].ic_feedback_trim * 0.9f );

            // Current torque feedback is similar to previous running average
            if( -1 * SERVO_HOMING_SIMILARITY_PERCENT < feedback_error || feedback_error < SERVO_HOMING_SIMILARITY_PERCENT )
            {
                // Check we've maintained this level for a minimum amount of time
                if( ( hal_systick_get_ms() - me->timer ) > SERVO_HOMING_SIMILARITY_MS )
                {
                    me->angle_current_steps = convert_angle_steps( -42.0f );
                    config_motor_target_angle( servo, -42.0f );    // update UI with angles before a target is sent in

                    me->angle_target_steps = me->angle_current_steps;
                    STATE_NEXT( SERVO_STATE_IDLE );
                }
            }
            else    // position is substantially different from previous trend
            {
                // Reset the similarity timer
                me->timer = hal_systick_get_ms();
            }

            //motor is taking too long to complete the homing process
            if( ( hal_systick_get_ms() - me->timer ) > SERVO_HOMING_COMPLETE_MAX_MS )
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
            if( me->angle_current_steps != me->angle_target_steps )
            {
                STATE_NEXT( SERVO_STATE_ACTIVE );
            }

            //allow some time for the motor to decelerate before we worry about excessive no-motion loads
            if( hal_systick_get_ms() - me->timer > SERVO_IDLE_SETTLE_MS )
            {
                //  Check if the motor has been drawing higher than expected power while stationary
                //  OR if the idle torque is above/below a generous bound
                if( servo_power > SERVO_IDLE_POWER_ALERT_W
                    || ( servo_feedback < -1 * SERVO_IDLE_TORQUE_ALERT && servo_feedback > SERVO_IDLE_TORQUE_ALERT ) )
                {
                    // Something might be wrong, watch it more closely
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
            if( me->angle_current_steps != me->angle_target_steps )
            {
                STATE_NEXT( SERVO_STATE_ACTIVE );
            }

            // Evaluate the power or torque values for high no-movement loads
            if( servo_power > SERVO_IDLE_POWER_ALERT_W
                || ( servo_feedback < -1 * SERVO_IDLE_TORQUE_ALERT && servo_feedback > SERVO_IDLE_TORQUE_ALERT ) )
            {
                //been measuring a pretty high load for a while now
                if( ( hal_systick_get_ms() - me->timer ) > SERVO_IDLE_LOAD_TRIP_MS )
                {
                    //shutdown for safety
                    config_report_error( "Servo Overload" );
                    eventPublish( EVENT_NEW( StateEvent, MOTION_EMERGENCY ) );
                    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                }
            }
            else
            {
                //if we've been under the alert threshold for a while, return to the idle state
                if( ( hal_systick_get_ms() - me->timer ) > SERVO_IDLE_LOAD_TRIP_MS * 2 )
                {
                    STATE_NEXT( SERVO_STATE_IDLE );
                }
            }

            // Been disabled or current sensor has flagged a fault, shutdown
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
            int8_t  step_direction  = 0;

            // Command rotation to move towards the target position
            if( step_difference != 0 )
            {
                // Command the target direction
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

                uint16_t pulses_needed = step_difference * step_direction;

                if( pulses_needed > 8 )
                {
                    pulses_needed = 8;
                    status_yellow( true );    // visual debugging aid to see when speed limits are hit
                }
                else
                {
                    status_yellow( false );
                }

                for( uint16_t pulses = 0; pulses < pulses_needed; pulses++ )
                {
                    hal_gpio_toggle_pin( ServoHardwareMap[servo].pin_step );
                    hal_delay_us( SERVO_PULSE_DURATION_US );
                    hal_gpio_toggle_pin( ServoHardwareMap[servo].pin_step );
                    hal_delay_us( SERVO_PULSE_DURATION_US );
                    me->angle_current_steps = me->angle_current_steps + ( step_direction * -1 );
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

    config_motor_state( servo, me->currentState );
    config_motor_enable( servo, me->enabled );
    config_motor_feedback( servo, servo_feedback );
    config_motor_power( servo, servo_power );
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
    float   converted_angle = kinematics_shoulder_angle + SERVO_MIN_ANGLE;
    int16_t angle_as_steps  = converted_angle * SERVO_STEPS_PER_DEGREE;

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
