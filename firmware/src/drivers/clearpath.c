/* ----- System Includes ---------------------------------------------------- */

#include <string.h>
#include <float.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "clearpath.h"
#include "sensors.h"

#include "hal_delay.h"
#include "hal_gpio.h"
#include "hal_ic_hard.h"

#include "app_times.h"
#include "global.h"
#include "qassert.h"
#include "simple_state_machine.h"
#include "average_short.h"
#include "timer_ms.h"
#include "user_interface.h"

/* ----- Defines ------------------------------------------------------------ */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

typedef enum
{
    SERVO_STATE_INACTIVE,
    SERVO_STATE_ERROR_RECOVERY,

    SERVO_STATE_HOMING_CALIBRATE_TORQUE,
    SERVO_STATE_HOMING_FIND_ENDSTOP,
    SERVO_STATE_HOMING_FOUND_ENDSTOP,
    SERVO_STATE_HOMING_CHECK_FOLDBACK,
    SERVO_STATE_HOMING_SUCCESS,

    SERVO_STATE_ACTIVE,
} ServoState_t;

typedef struct
{
    ServoState_t previousState;
    ServoState_t currentState;
    ServoState_t nextState;

    bool       enabled;

    float      ic_feedback_trim;
    float      homing_feedback;
    timer_ms_t timer;
    int32_t    angle_current_steps;
    int32_t    angle_target_steps;

    AverageShort_t step_statistics;
    bool       presence_detected;
    bool       has_high_load;
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

typedef struct
{
    bool installed;
    bool requires_homing;
    bool reverse_direction;
    uint32_t steps_per_revolution;
    float ratio;
    float angle_min;
    float angle_max;
    float angle_at_home;
} ServoConfiguration_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Servo_t clearpath[_NUMBER_CLEARPATH_SERVOS];

PRIVATE const ServoHardware_t ServoHardwareMap[_NUMBER_CLEARPATH_SERVOS] = {
    [_CLEARPATH_1] = { .pin_enable      = _SERVO_1_ENABLE,
                       .pin_direction   = _SERVO_1_A,
                       .pin_step        = _SERVO_1_B,
                       .pin_feedback    = _SERVO_1_HLFB,
                       .ic_feedback     = HAL_IC_HARD_HLFB_SERVO_1,
                       .adc_current     = HAL_ADC_INPUT_M1_CURRENT,
                       .pin_oc_fault    = _SERVO_1_CURRENT_FAULT },

    [_CLEARPATH_2] = { .pin_enable      = _SERVO_2_ENABLE,
                       .pin_direction   = _SERVO_2_A,
                       .pin_step        = _SERVO_2_B,
                       .pin_feedback    = _SERVO_2_HLFB,
                       .ic_feedback     = HAL_IC_HARD_HLFB_SERVO_2,
                       .adc_current     = HAL_ADC_INPUT_M2_CURRENT,
                       .pin_oc_fault    = _SERVO_2_CURRENT_FAULT },

    [_CLEARPATH_3] = { .pin_enable      = _SERVO_3_ENABLE,
                       .pin_direction   = _SERVO_3_A,
                       .pin_step        = _SERVO_3_B,
                       .pin_feedback    = _SERVO_3_HLFB,
                       .ic_feedback     = HAL_IC_HARD_HLFB_SERVO_3,
                       .adc_current     = HAL_ADC_INPUT_M3_CURRENT,
                       .pin_oc_fault    = _SERVO_3_CURRENT_FAULT },


    [_CLEARPATH_4] = { .pin_enable    = _SERVO_4_ENABLE,
                       .pin_direction = _SERVO_4_A,
                       .pin_step      = _SERVO_4_B,
                       .pin_feedback  = _SERVO_4_HLFB,
                       .ic_feedback   = HAL_IC_HARD_HLFB_SERVO_4,
                       .adc_current   = HAL_ADC_INPUT_M4_CURRENT,
                       .pin_oc_fault  = _SERVO_4_CURRENT_FAULT },
};

PRIVATE ServoConfiguration_t ServoConfig[_NUMBER_CLEARPATH_SERVOS] = {
    [_CLEARPATH_1] = { .installed            = true,
                       .requires_homing      = true,
                       .reverse_direction    = false,
                       .steps_per_revolution = SERVO_STEPS_PER_REV,
                       .ratio                = 1.0f,
                       .angle_min            = -1*SERVO_MIN_ANGLE,
                       .angle_max            = SERVO_MAX_ANGLE,
                       .angle_at_home        = -42.0f },

    [_CLEARPATH_2] = { .installed            = true,
                       .requires_homing      = true,
                       .reverse_direction    = false,
                       .steps_per_revolution = SERVO_STEPS_PER_REV,
                       .ratio                = 1.0f,
                       .angle_min            = -1*SERVO_MIN_ANGLE,
                       .angle_max            = SERVO_MAX_ANGLE,
                       .angle_at_home        = -42.0f },

    [_CLEARPATH_3] = { .installed            = true,
                       .requires_homing      = true,
                       .reverse_direction    = false,
                       .steps_per_revolution = SERVO_STEPS_PER_REV,
                       .ratio                = 1.0f,
                       .angle_min            = -1*SERVO_MIN_ANGLE,
                       .angle_max            = SERVO_MAX_ANGLE,
                       .angle_at_home        = -42.0f },

    [_CLEARPATH_4] = { .installed            = false,
                       .requires_homing      = false,
                       .reverse_direction    = false,
                       .steps_per_revolution = 0,
                       .ratio                = 0,
                       .angle_min            = 0,
                       .angle_max            = 0,
                       .angle_at_home        = 0 },
};

PRIVATE float servo_get_hlfb_percent( ClearpathServoInstance_t servo );

PRIVATE float servo_get_hlfb_percent_corrected( ClearpathServoInstance_t servo );

PRIVATE bool servo_get_connected_estimate( ClearpathServoInstance_t servo );

PRIVATE int32_t convert_angle_steps( ClearpathServoInstance_t servo, float angle );

PRIVATE float convert_steps_angle( ClearpathServoInstance_t servo, int32_t steps );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
servo_init( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );

    memset( &clearpath[servo], 0, sizeof( Servo_t ) );

    // Buffer commanded steps per tick for 50 ticks (50ms) to back velocity estimate
    average_short_init( &clearpath[servo].step_statistics, SPEED_ESTIMATOR_SPAN );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_is_configured( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    return ( ServoConfig[servo].installed && ServoConfig[servo].ratio >= 0.0f + FLT_EPSILON );
}

/* -------------------------------------------------------------------------- */

// Returns the number of servos that have valid configuration
//  implies that this number servos are ready for use
PUBLIC uint8_t
servo_get_configured_count( void )
{
    uint8_t num_configured = 0;

    for( ClearpathServoInstance_t servo = 0; servo < _NUMBER_CLEARPATH_SERVOS; servo++ )
    {
        num_configured += servo_is_configured(servo);
    }

    return num_configured;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_change_configuration( ClearpathServoInstance_t servo,
                            bool is_installed,
                            bool requires_homing,
                            bool reverse_direction,
                            uint32_t steps_per_revolution,
                            float ratio,
                            float angle_min,
                            float angle_max,
                            float angle_at_home )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );

    ServoConfiguration_t *config = &ServoConfig[servo];

    // TODO: consider doing bounds/error checks on these values?
    config->installed = is_installed;
    config->requires_homing = requires_homing;
    config->reverse_direction = reverse_direction;
    config->steps_per_revolution = steps_per_revolution;
    config->ratio = ratio;
    config->angle_min = angle_min;
    config->angle_max = angle_max;
    config->angle_at_home = angle_at_home;

    return false;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_start( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    me->enabled = SERVO_ENABLE;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_stop( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    me->enabled = SERVO_DISABLE;
}

/* -------------------------------------------------------------------------- */

// Immediately disable all servo's enable pins
//   Intended for ASSERT style 'disable the servos' end-of-the-world style use, not general disable
PUBLIC void
servo_disable_all_hard( void )
{
    hal_gpio_write_pin( ServoHardwareMap[_CLEARPATH_1].pin_enable, SERVO_DISABLE );
    hal_gpio_write_pin( ServoHardwareMap[_CLEARPATH_2].pin_enable, SERVO_DISABLE );
    hal_gpio_write_pin( ServoHardwareMap[_CLEARPATH_3].pin_enable, SERVO_DISABLE );
    hal_gpio_write_pin( ServoHardwareMap[_CLEARPATH_4].pin_enable, SERVO_DISABLE );
}

/* -------------------------------------------------------------------------- */

// Calculates and sets target position, constrains input to legal angles only
PUBLIC void
servo_set_target_angle_limited( ClearpathServoInstance_t servo, float angle_degrees )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    user_interface_motor_target_angle( servo, angle_degrees );

    if( angle_degrees > ServoConfig[servo].angle_min && angle_degrees < ServoConfig[servo].angle_max )
    {
        me->angle_target_steps = convert_angle_steps( servo, angle_degrees );
    }
}

/* -------------------------------------------------------------------------- */

// Calculates and sets the target position in steps without checking legality of request
PUBLIC void
servo_set_target_angle_raw( ClearpathServoInstance_t servo, float angle_degrees )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    user_interface_motor_target_angle( servo, angle_degrees );

    // TODO fix/rework obsolete
    const uint32_t steps_per_degree = ( 400 / SERVO_ANGLE_PER_REV );
    me->angle_target_steps          = steps_per_degree * angle_degrees;
}

/* -------------------------------------------------------------------------- */

PUBLIC float
servo_get_current_angle( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    return convert_steps_angle( servo, me->angle_current_steps );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint16_t
servo_get_steps_per_second( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    // The scalar sum of steps commanded for a span covering 50ms
    uint16_t step_sum = average_short_get_sum( &me->step_statistics );

    // Report as 'steps per second'
    return step_sum * 20;
}

PUBLIC float
servo_get_degrees_per_second( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    return convert_steps_angle( servo, servo_get_steps_per_second( servo ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_get_move_done( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    return ( me->angle_current_steps == me->angle_target_steps );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_get_servo_ok( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    return (    me->enabled
             && me->currentState == SERVO_STATE_ACTIVE
             && !me->has_high_load );
}

PUBLIC bool
servo_get_servo_did_error( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    return ( me->currentState == SERVO_STATE_ERROR_RECOVERY || me->previousState == SERVO_STATE_ERROR_RECOVERY || me->nextState == SERVO_STATE_ERROR_RECOVERY );
}

/* -------------------------------------------------------------------------- */

// Returns uncorrected servo feedback torque as a percentage from -100% to 100% of rated capability
PRIVATE float
servo_get_hlfb_percent( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );

    float percentage = 0.0f;

    // HLFB from servos is a square-wave where 5% < x < 95% is used for torque/speed output
    // The hardware input capture driver returns us the value as %duty cycle
    // 65% DC => 1/3rd max torque in +ve direction

    // Get the HLFB duty, and scale to -100% to +100% range
    percentage = hal_ic_hard_read_f( ServoHardwareMap[servo].ic_feedback ) * 2.05f - 100.0f;
    percentage = CLAMP( percentage, -100.0f, 100.0f );

    return percentage;
}

// Corrected servo feedback uses a trim value calculated during the arming procedure
PRIVATE float
servo_get_hlfb_percent_corrected( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    return servo_get_hlfb_percent( servo ) - clearpath[servo].ic_feedback_trim;
}

// By checking that HLFB pulses were caught by the input capture recently,
// can assume that a servo is connected or disconnected
PRIVATE bool
servo_get_connected_estimate( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );

    uint32_t ms_since_last = hal_ic_hard_ms_since_value( ServoHardwareMap[servo].ic_feedback );
    return ( ms_since_last < SERVO_MISSING_HLFB_MS );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_process( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );

    // Early exit if not configured
    if( !ServoConfig[servo].installed )
    {
        return;
    }

    Servo_t *me = &clearpath[servo];

    float servo_power    = sensors_servo_W( ServoHardwareMap[servo].adc_current );
    float servo_feedback = servo_get_hlfb_percent_corrected( servo );

    // Check if the servo has provided HLFB signals as proxy for 'detection'
    me->presence_detected = servo_get_connected_estimate( servo );

    // Servo feedback loss in any active triggers immediate error handling behaviour
    if( !me->presence_detected && me->enabled )
    {
        STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
    }

    // If disabled (by supervisor etc) then immediately start disable process
    if(    ( me->enabled == SERVO_DISABLE )
        && ( me->currentState > SERVO_STATE_ERROR_RECOVERY ) )
    {
        STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
    }

    switch( me->currentState )
    {
        case SERVO_STATE_INACTIVE:
            STATE_ENTRY_ACTION
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, SERVO_DISABLE );
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_step, false );
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, false );
            me->enabled = SERVO_DISABLE;
            STATE_TRANSITION_TEST
            if( me->enabled && me->presence_detected )
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
            timer_ms_start( &me->timer, SERVO_FAULT_LINGER_MS );
            STATE_TRANSITION_TEST

            if( timer_ms_is_expired( &me->timer ) )
            {
                STATE_NEXT( SERVO_STATE_INACTIVE );
            }

            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING_CALIBRATE_TORQUE:
            STATE_ENTRY_ACTION
            clearpath[servo].ic_feedback_trim = 0.0f;
            timer_ms_start( &me->timer, SERVO_HOMING_CALIBRATION_MS );
            STATE_TRANSITION_TEST
            float uncorrected_feedback = servo_get_hlfb_percent( servo );

            if( !timer_ms_is_expired( &me->timer ) )
            {
                // The trim value is a super simple average of the incoming values
                clearpath[servo].ic_feedback_trim = ( 0.1f * uncorrected_feedback ) + ( clearpath[servo].ic_feedback_trim * 0.9f );
            }
            else
            {
                // Check that the corrected feedback value with our trim is pretty close to zero
                if( -0.5f < servo_feedback && servo_feedback < 0.5f )
                {
                    if( ServoConfig[servo].requires_homing )
                    {
                        STATE_NEXT( SERVO_STATE_HOMING_FIND_ENDSTOP );
                    }
                    else
                    {
                        // Just enable the servo then 'torque settle' validation
                        hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, SERVO_ENABLE );
                        STATE_NEXT( SERVO_STATE_HOMING_SUCCESS );
                    }

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
            // EN pin high starts the automatic homing process on the Clearpath servo.
            // Servo homing behaviour is defined with the Clearpath setup software, uses torque based end-stop sensing
            hal_gpio_write_pin( ServoHardwareMap[servo].pin_enable, SERVO_ENABLE );

            //            timer_ms_start( &me->timer, 999 );
            STATE_TRANSITION_TEST
            // We expect the torque to be positive while the arm transits towards the endstop
            // When it goes under 0% HLFB, the torque has changed direction and we've likely just contacted the endstop
            if( servo_feedback < -1.0f )
            {
                // After this point, we'd expect the torque to increase (in -ve direction) until foldback kicks in
                STATE_NEXT( SERVO_STATE_HOMING_FOUND_ENDSTOP );
            }

            // TODO timeout if we don't transit there in some reasonable amount of time?
            // timer_ms_is_expired( ... )
            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING_FOUND_ENDSTOP:
            STATE_ENTRY_ACTION
            me->homing_feedback = servo_feedback;
            STATE_TRANSITION_TEST
            // We think the servo has found the endstop. Torque should be increasing until servo hits the homing threshold.
            // Once the servo reaches its thresholded homing torque, torque foldback kicks in
            if( servo_feedback < me->homing_feedback )
            {
                // increasing torque in -ve direction
                me->homing_feedback = servo_feedback;
            }
            else
            {
                // Found a local peak, check it's over the servo's expected threshold + some error padding
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
            timer_ms_stopwatch_start( &me->timer );
            STATE_TRANSITION_TEST
            // Check that the servo is loaded against the endstop in foldback mode
            // When we enter this state, foldback is likely still heading to setpoint
            // Check the value is reasonable for a continuous slice of time
            if( timer_ms_stopwatch_lap( &me->timer ) > SERVO_HOMING_FOLDBACK_CHECK_START_MS )
            {
                // Check if foldback holding torque value is outside the expected range...
                if( servo_feedback > -1 * SERVO_HOMING_FOLDBACK_TORQUE_MIN
                    && servo_feedback < -1 * SERVO_HOMING_FOLDBACK_TORQUE_MAX )
                {

                    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                }

                // If we made it this far while being in range, everything looks good
                if( timer_ms_stopwatch_lap( &me->timer ) < SERVO_HOMING_FOLDBACK_CHECK_END_MS )
                {
                    STATE_NEXT( SERVO_STATE_HOMING_SUCCESS );
                }
            }
            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_HOMING_SUCCESS:
            STATE_ENTRY_ACTION
            timer_ms_stopwatch_start( &me->timer );
            me->homing_feedback = 0.0f;
            STATE_TRANSITION_TEST
            // Servo successfully found home, and is likely moving to the start-offset point
            // Wait for the servo to settle in torque domain
            float feedback_error = ( servo_feedback - me->homing_feedback );
            me->homing_feedback  = ( 0.1f * servo_feedback ) + ( clearpath[servo].ic_feedback_trim * 0.9f );

            // Current torque feedback is similar to previous running average
            if( -1 * SERVO_HOMING_SIMILARITY_PERCENT < feedback_error
                || feedback_error < SERVO_HOMING_SIMILARITY_PERCENT )
            {
                // Check we've maintained this level for a minimum amount of time
                if( timer_ms_stopwatch_lap( &me->timer ) > SERVO_HOMING_SIMILARITY_MS )
                {
                    // The servo's internal homing procedure can be configured to move to a position after homing
                    // This angle is what this system would interpret that position as
                    float home_angle = ServoConfig[servo].angle_at_home;
                    me->angle_current_steps = convert_angle_steps( servo, home_angle );
                    user_interface_motor_target_angle( servo, home_angle );    // update UI with angles before a target is sent in

                    // Goal position needs to be the current position at init, or a large commanded move would occur
                    me->angle_target_steps = me->angle_current_steps;
                    me->has_high_load = false;

                    STATE_NEXT( SERVO_STATE_ACTIVE );
                }
            }
            else    // Position is substantially different from previous trend
            {
                // Reset the similarity timer
                timer_ms_stopwatch_start( &me->timer );
            }

            // Motor is taking too long to complete the homing process
            if( timer_ms_stopwatch_lap( &me->timer ) > SERVO_HOMING_COMPLETE_MAX_MS )
            {
                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
            }
            STATE_EXIT_ACTION

            STATE_END
            break;

        case SERVO_STATE_ACTIVE:
            STATE_ENTRY_ACTION

            STATE_TRANSITION_TEST
            int32_t step_difference = me->angle_current_steps - me->angle_target_steps;

            // Command rotation to move towards the target position
            if( step_difference != 0 )
            {
                // Command direction is calculated by finding the polarity of the target error
                // then XOR with the user's configuration to support 'flipping' direction
                bool direction_ccw = ( me->angle_current_steps > me->angle_target_steps ) ^ ServoConfig[servo].reverse_direction;
                int8_t step_direction = ( direction_ccw ) ? 1 : -1;

                hal_gpio_write_pin( ServoHardwareMap[servo].pin_direction, direction_ccw );

                // Clamp the number of steps per process loop's 'millisecond tick'
                uint32_t pulses_needed = MIN( step_difference * step_direction, 8 );

                for( uint32_t pulses = 0; pulses < pulses_needed; pulses++ )
                {
                    hal_gpio_toggle_pin( ServoHardwareMap[servo].pin_step );
                    hal_delay_us( SERVO_PULSE_DURATION_US );
                    hal_gpio_toggle_pin( ServoHardwareMap[servo].pin_step );
                    hal_delay_us( SERVO_PULSE_DURATION_US );
                    me->angle_current_steps = me->angle_current_steps + ( step_direction * -1 );
                }

                // Track movement requests over time for velocity estimate
                average_short_update( &me->step_statistics, pulses_needed );

                // Reset the 'no-motion' idle timer
                timer_ms_start( &me->timer, SERVO_IDLE_SETTLE_MS );
            }
            else    // no movement needed
            {
                average_short_update( &me->step_statistics, 0 );

                // Has the servo been idle for longer than the settling period?
                if( timer_ms_is_expired( &me->timer ) )
                {
                    // Are the power or torque values high for a no-movement load?
                    if( servo_power > SERVO_IDLE_POWER_ALERT_W
                        || ( servo_feedback < -1 * SERVO_IDLE_TORQUE_ALERT && servo_feedback > SERVO_IDLE_TORQUE_ALERT ) )
                    {
                        me->has_high_load = true;
                    }
                }
            }

            if( hal_gpio_read_pin( ServoHardwareMap[servo].pin_oc_fault ) == SERVO_OC_FAULT )
            {
                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
            }
            STATE_EXIT_ACTION

            STATE_END
            break;
    }

    // Continue updating count stats when not actively driving motor, to ensure velocity stats include stationary time
    if( me->currentState != SERVO_STATE_ACTIVE )
    {
        average_short_update( &me->step_statistics, 0 );
    }

    user_interface_motor_state( servo, me->currentState );
    user_interface_motor_enable( servo, me->enabled );
    user_interface_motor_feedback( servo, servo_feedback );
    user_interface_motor_power( servo, servo_power );
    user_interface_motor_speed( servo, servo_get_degrees_per_second(servo) );
}

/* -------------------------------------------------------------------------- */

// TODO when working out why angle_min is involved, rewrite this comment
/*
 * The kinematics output is an angle between -85 and +90, where 0 deg is when
 * the elbow-shaft link is parallel to the frame plate. Full range not available due
 * to physical limits on arm travel. Approx -45 to +70 is the safe working range.
 *
 * Servo steps are referenced to the homing point, which is the HW minimum,
 * therefore we need to convert the angle into steps, and apply the adequate offset.
 * The servo's range is approx 2300 counts.
 */
PRIVATE int32_t
convert_angle_steps( ClearpathServoInstance_t servo, float angle )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    REQUIRE( ServoConfig[servo].ratio > 0.0f + FLT_EPSILON );

    float steps_per_servo_degree = (float)ServoConfig[servo].steps_per_revolution / SERVO_ANGLE_PER_REV;
    float output_ratio = ServoConfig[servo].ratio;

    // TODO remove/work out why angle_min is involved here at all?
    float servo_angle = ( angle + ServoConfig[servo].angle_min );

    float converted_angle  = servo_angle * output_ratio * steps_per_servo_degree;
    return (int32_t)converted_angle;
}

/* -------------------------------------------------------------------------- */

/*
 * Convert a motor position in steps to an angle in the motor reference frame
 */
PRIVATE float
convert_steps_angle( ClearpathServoInstance_t servo, int32_t steps )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    REQUIRE( ServoConfig[servo].ratio > 0.0f + FLT_EPSILON );

    float steps_per_degree = (float)ServoConfig[servo].steps_per_revolution / SERVO_ANGLE_PER_REV;
    float output_ratio = ServoConfig[servo].ratio;

    float steps_to_angle = (float)steps / steps_per_degree / output_ratio;
    return steps_to_angle;
}

/* ----- End ---------------------------------------------------------------- */