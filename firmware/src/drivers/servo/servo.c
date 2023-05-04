/* -------------------------------------------------------------------------- */

#include <float.h>
#include <string.h>

#include "global.h"
#include "qassert.h"
#include "signals.h"
#include "simple_state_machine.h"

#include "servo.h"
#include "observer.h"
#include "hal_gpio.h"

#include "stopwatch.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

#define SERVO_STEPS_PER_REV (6400U)
#define SERVO_ANGLE_PER_REV (360U)
#define SERVO_MIN_ANGLE (45U)  // this is the negative angle limit
#define SERVO_MAX_ANGLE (65U)  // arm is fully extended
#define SERVO_IDLE_POWER_ALERT_W (40U)
#define SERVO_IDLE_TORQUE_ALERT (30U)
#define SERVO_IDLE_SETTLE_MS (50U)
#define SERVO_HOME_OFFSET (25U)

// ms since last HLFB input captures before declaring the servo missing
// As HLFB is 45Hz = 22ms, timeout is set to allow two missed cycles
#define SERVO_MISSING_HLFB_MS (50U)

// Clearpath input high = clockwise rotation. Alias against pin state
#define SERVO_DIR_CCW (true)
#define SERVO_DIR_CW (false)

#define SERVO_HOMING_ENDSTOP_TRANSIT_TORQUE     (20U)     // shutdown if servo exceeds this limit during the move towards the endstop
#define SERVO_HOMING_ENDSTOP_RAMP_MIN           (15U)     // lower limit % of torque expected to trigger homing endstop behaviour
#define SERVO_HOMING_ENDSTOP_RAMP_MAX           (25U)     // upper limit % of torque expected to home
#define SERVO_HOMING_FOLDBACK_CHECK_START_MS    (200U)    // ms after peak to start checking foldback level
#define SERVO_HOMING_FOLDBACK_CHECK_END_MS      (500U)    // ms after peak to check foldback is still ok
#define SERVO_HOMING_FOLDBACK_TORQUE_MIN        (4U)      // % torque min expected during foldback
#define SERVO_HOMING_FOLDBACK_TORQUE_MAX        (6U)      // % torque max expected during foldback
#define SERVO_HOMING_SIMILARITY_PERCENT         (1U)      // % torque error allowed during stabilisation period
#define SERVO_HOMING_SIMILARITY_MS              (200U)    // time the torque needs to be stable before considering homing move complete
#define SERVO_HOMING_COMPLETE_MAX_MS            (500U)
#define SERVO_HOMING_MAX_MS                     (9000U)

#define SERVO_HOMING_CALIBRATION_SAMPLES  (20U)
#define SERVO_HOMING_CALIBRATION_MS       (SERVO_HOMING_CALIBRATION_SAMPLES * 22U)  // 45hz -> 22ms per sample
#define SERVO_HOMING_CALIBRATION_TORQUE   (10U)                                     // maximum torque we expect to see during torque calibration

#define SERVO_RECOVERY_DWELL_MS (50U)
#define SERVO_RECOVERY_RETRIES  (7U)

// Clearpath will filter pulses shorter than 1us
// ULN2303 NPN driver has rise time of ~5ns, fall of ~10nsec
#define SERVO_PULSE_DURATION_US (10U)

// Fault handling
#define SERVO_FAULT_LINGER_MS (500U)

// ms since last HLFB input captures before declaring the servo missing
// As HLFB is 45Hz = 22ms, timeout is set to allow two missed cycles
#define SERVO_MISSING_HLFB_MS (50U)


/* -------------------------------------------------------------------------- */

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
    // Servo IO
    HalGpioPortPin_t pin_enable;
    HalGpioPortPin_t pin_direction;
    HalGpioPortPin_t pin_step;
    HalGpioPortPin_t pin_oc_fault;

    // Subscribe to these event flags
    SENSOR_EVENT_FLAG sensor_current;   // Current sense IC
    SENSOR_EVENT_FLAG sensor_hlfb;      // HLFB (HighLevelFeedBack)

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

typedef struct
{
    ServoState_t previousState;
    ServoState_t currentState;
    ServoState_t nextState;

    ServoHardware_t hardware;
    ServoConfiguration_t config;

    bool       enabled;

    float      ic_feedback_trim;
    float      homing_feedback;
    stopwatch_t   timer;
    int32_t    angle_current_steps;
    int32_t    angle_target_steps;

    SemaphoreHandle_t xServoUpdateSemaphore;
    Observer sensor_observer;
    float current;
    float hlfb;     // servo feedback includes the ic_feedback_trim value calculated during homing

//    AverageShort_t step_statistics;
    bool       presence_detected;
    bool       has_high_load;
} Servo_t;

/* -------------------------------------------------------------------------- */

PRIVATE void servo_load_configuration( ClearpathServoInstance_t servo );

PRIVATE void servo_load_hardware( ClearpathServoInstance_t servo);

PRIVATE void servo_sensors_callback( ObserverEvent_t event, EventData data, void *context );

PRIVATE bool servo_get_connected_estimate( ClearpathServoInstance_t servo );

PRIVATE int32_t convert_angle_steps( Servo_t *me, float angle );

PRIVATE float convert_steps_angle( Servo_t *me, int32_t steps );

/* -------------------------------------------------------------------------- */

PRIVATE Servo_t clearpath[_NUMBER_CLEARPATH_SERVOS];

PRIVATE SemaphoreHandle_t xClearpathMutex;

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_init( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    memset( me, 0, sizeof( Servo_t ) );
    servo_load_configuration( servo );
    servo_load_hardware( servo );

    // Setup the 'task needs to run' semaphore
    me->xServoUpdateSemaphore = xSemaphoreCreateBinary();
    ENSURE( me->xServoUpdateSemaphore );

    // There's only one, so the first servo to init can create it.
    if( !xClearpathMutex )
    {
        xClearpathMutex = xSemaphoreCreateMutex();
    }
    ENSURE( me->xServoUpdateSemaphore );

    // Setup sensor event subscriptions
    observer_init( &me->sensor_observer, servo_sensors_callback, &clearpath[servo] );
    observer_subscribe( &me->sensor_observer, me->hardware.sensor_current );
    observer_subscribe( &me->sensor_observer, me->hardware.sensor_hlfb );

    // Buffer commanded steps per tick for 50 ticks (50ms) to back velocity estimate
//    average_short_init( &clearpath[servo].step_statistics, SPEED_ESTIMATOR_SPAN );
}


PUBLIC void* servo_get_state_context_for( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    return (void*)&clearpath[servo];
}

PUBLIC Observer* servo_get_observer( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    return &clearpath[servo].sensor_observer;
}

/* -------------------------------------------------------------------------- */

PRIVATE void servo_sensors_callback(ObserverEvent_t event, EventData data, void *context)
{
    // Gain access to effector data
    // TODO: how long should we wait for the mutex?
    if( xSemaphoreTake( xClearpathMutex, portMAX_DELAY) )
    {
        Servo_t *me = context;
        REQUIRE( me );

        switch( event )
        {
            case SENSOR_SERVO_1_CURRENT:
            case SENSOR_SERVO_2_CURRENT:
            case SENSOR_SERVO_3_CURRENT:
            case SENSOR_SERVO_4_CURRENT:
                me->current = data.floatValue;
                xSemaphoreGive( me->xServoUpdateSemaphore );
                break;

            case SENSOR_SERVO_1_HLFB:
            case SENSOR_SERVO_2_HLFB:
            case SENSOR_SERVO_3_HLFB:
            case SENSOR_SERVO_4_HLFB:
                me->hlfb = data.floatValue;
                xSemaphoreGive( me->xServoUpdateSemaphore );
                // - me->ic_feedback_trim;  // TODO: is 'correcting' the value here the right thing to do?
                break;

            default:
                // Why did we recieve a signal that we didn't subscribe to?
                ASSERT(false);
                break;
        }
        xSemaphoreGive( xClearpathMutex );
    }

}

/* -------------------------------------------------------------------------- */

PUBLIC bool
servo_is_configured( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    return ( me->config.installed && me->config.ratio >= 0.0f + FLT_EPSILON );
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

    // TODO: how long should we wait for the mutex?
    if( xSemaphoreTake( xClearpathMutex, portMAX_DELAY) )
    {
        Servo_t *me = &clearpath[servo];

        ServoConfiguration_t *config = &me->config;

        // TODO: consider doing bounds/error checks on these values?
        config->installed            = is_installed;
        config->requires_homing      = requires_homing;
        config->reverse_direction    = reverse_direction;
        config->steps_per_revolution = steps_per_revolution;
        config->ratio                = ratio;
        config->angle_min            = angle_min;
        config->angle_max            = angle_max;
        config->angle_at_home        = angle_at_home;
        xSemaphoreGive( xClearpathMutex );

        return true;
    }

    return false;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_enable( ClearpathServoInstance_t servo, bool enabled )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );

    // TODO: how long should we wait for the mutex?
    if( xSemaphoreTake( xClearpathMutex, portMAX_DELAY) )
    {
        Servo_t *me = &clearpath[servo];
        me->enabled = enabled;
        xSemaphoreGive( me->xServoUpdateSemaphore );

        xSemaphoreGive( xClearpathMutex );
    }

}

/* -------------------------------------------------------------------------- */

// Immediately disable all servo's enable pins
//   Intended for ASSERT style 'disable the servos' end-of-the-world style use, not general disable
PUBLIC void
servo_disable_all_hard( void )
{
    hal_gpio_write_pin( clearpath[_CLEARPATH_1].hardware.pin_enable, false );
    hal_gpio_write_pin( clearpath[_CLEARPATH_2].hardware.pin_enable, false );
    hal_gpio_write_pin( clearpath[_CLEARPATH_3].hardware.pin_enable, false );
    hal_gpio_write_pin( clearpath[_CLEARPATH_4].hardware.pin_enable, false );
}

/* -------------------------------------------------------------------------- */

// Calculates and sets target position, constrains input to legal angles only
PUBLIC void
servo_set_target_angle_limited( ClearpathServoInstance_t servo, float angle_degrees )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );

    // TODO: how long should we wait for the mutex?
    if( xSemaphoreTake( xClearpathMutex, portMAX_DELAY) )
    {
        Servo_t *me = &clearpath[servo];

        if( angle_degrees > me->config.angle_min && angle_degrees < me->config.angle_max )
        {
            me->angle_target_steps = convert_angle_steps( me, angle_degrees );
        }

        xSemaphoreGive( xClearpathMutex );
    }
}

/* -------------------------------------------------------------------------- */

// Calculates and sets the target position in steps without checking legality of request
PUBLIC void
servo_set_target_angle_raw( ClearpathServoInstance_t servo, float angle_degrees )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

//    user_interface_motor_target_angle( servo, angle_degrees );

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

    return convert_steps_angle( me, me->angle_current_steps );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint16_t
servo_get_steps_per_second( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    // TODO rework these stats entirely?
    // The scalar sum of steps commanded for a span covering 50ms
    uint16_t step_sum = 10; //average_short_get_sum( &me->step_statistics );

    // Report as 'steps per second'
    return step_sum * 20;
}

PUBLIC float
servo_get_degrees_per_second( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    return convert_steps_angle( me, servo_get_steps_per_second( servo ) );
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

// By checking that HLFB pulses were caught by the input capture recently,
// can assume that a servo is connected or disconnected
PRIVATE bool
servo_get_connected_estimate( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );

    uint32_t ms_since_last = 10;    // hal_ic_hard_ms_since_value( ServoHardwareMap[servo].ic_feedback );
    return ( ms_since_last < SERVO_MISSING_HLFB_MS );
}

/* -------------------------------------------------------------------------- */

PUBLIC void servo_task( void* arg )
{
    Servo_t *me = arg;
    ENSURE( me );

    // Early exit if not configured
    if( !me->config.installed )
    {
        return;
    }

    for(;;)
    {
        // Wait for a command or new sensor data
        // TODO: set a safety timeout if nothing arrives often enough
        if( xSemaphoreTake( me->xServoUpdateSemaphore, portMAX_DELAY) )
        {
            // TODO: how long should we wait for the mutex?
            if( xSemaphoreTake( xClearpathMutex, portMAX_DELAY) )
            {

                // TODO: we don't actually care about current? so consider adding power (watts) outputs to the sensor subject?
                // TODO: fix assumption of 75V supply voltage to servos, see above note.
                float servo_power    = me->current * 75.0f;
                float servo_feedback = me->hlfb - me->ic_feedback_trim;

                // Check if the servo has provided HLFB signals as proxy for 'detection'
                // TODO fix 'servo connected' detection once sensorSubject can send timestamps?
                me->presence_detected = true;    // servo_get_connected_estimate( servo );

                // Servo feedback loss in any active state should trigger immediate error handling behaviour
                if( !me->presence_detected && me->enabled )
                {
                    //        user_interface_report_error( "Servo HLFB timeout" );
                    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                }

                // If disabled (by supervisor etc) then immediately start disable process
                if( ( !me->enabled )
                    && ( me->currentState > SERVO_STATE_ERROR_RECOVERY ) )
                {
                    STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                }

                switch( me->currentState )
                {
                    case SERVO_STATE_INACTIVE:
                        STATE_ENTRY_ACTION
                        hal_gpio_write_pin( me->hardware.pin_enable, false );
                        hal_gpio_write_pin( me->hardware.pin_step, false );
                        hal_gpio_write_pin( me->hardware.pin_direction, false );
                        me->enabled = false;
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
                        hal_gpio_write_pin( me->hardware.pin_enable, false );
                        hal_gpio_write_pin( me->hardware.pin_step, false );
                        hal_gpio_write_pin( me->hardware.pin_direction, false );

                        me->enabled = false;
                        stopwatch_deadline_start( &me->timer, SERVO_FAULT_LINGER_MS );
                        STATE_TRANSITION_TEST

                        if( stopwatch_deadline_elapsed( &me->timer ) )
                        {
                            STATE_NEXT( SERVO_STATE_INACTIVE );
                        }

                        STATE_EXIT_ACTION

                        STATE_END
                        break;

                    case SERVO_STATE_HOMING_CALIBRATE_TORQUE:
                        STATE_ENTRY_ACTION
                        me->ic_feedback_trim = 0.0f;
                        stopwatch_deadline_start( &me->timer, SERVO_HOMING_CALIBRATION_MS );
                        STATE_TRANSITION_TEST

                        if( !stopwatch_deadline_elapsed( &me->timer ) )
                        {
                            // The trim value is a super simple FIR of the incoming values
                            me->ic_feedback_trim = ( 0.1f * me->hlfb ) + ( me->ic_feedback_trim * 0.9f );
                        }
                        else
                        {
                            // Check that the corrected feedback value with our trim is pretty close to zero
                            if( IS_IN_DEADBAND( servo_feedback, 0.0f, 0.5f ) )
                            {
                                if( me->config.requires_homing )
                                {
                                    STATE_NEXT( SERVO_STATE_HOMING_FIND_ENDSTOP );
                                }
                                else
                                {
                                    // Just enable the servo then 'torque settle' validation
                                    hal_gpio_write_pin( me->hardware.pin_enable, true );
                                    STATE_NEXT( SERVO_STATE_HOMING_SUCCESS );
                                }
                            }
                            else
                            {
                                // An average of the disabled motor's feedback somehow didn't offset an error
                                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                            }
                        }

                        // Large uncalibrated torque values (+- from 0%) during calibration could be a hardware issue
                        if( !IS_IN_DEADBAND( me->hlfb, 0.0f, SERVO_HOMING_CALIBRATION_TORQUE ) )
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
                        hal_gpio_write_pin( me->hardware.pin_enable, true );
                        stopwatch_deadline_start( &me->timer, SERVO_HOMING_MAX_MS );

                        STATE_TRANSITION_TEST
                        // We expect the torque to be positive while the arm transits towards the endstop
                        // When it goes under 0% HLFB, the torque has changed direction and we've likely just contacted the endstop
                        if( servo_feedback < -1.0f )
                        {
                            // After this point, we'd expect the torque to increase (in -ve direction) until foldback kicks in
                            STATE_NEXT( SERVO_STATE_HOMING_FOUND_ENDSTOP );
                        }

                        // Timeout after a very long time without seeing the current peak
                        if( stopwatch_deadline_elapsed( &me->timer ) )
                        {
                            STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                        }
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
                                if( servo_feedback < ( -1.0f * SERVO_HOMING_ENDSTOP_RAMP_MIN ) )
                                {
                                    STATE_NEXT( SERVO_STATE_HOMING_CHECK_FOLDBACK );
                                }
                            }
                            float limit = ( -1.0f * SERVO_HOMING_ENDSTOP_RAMP_MAX );
                            // Error if the homing force is higher than expected
                            if( servo_feedback < limit )
                            {
                                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                            }
                            STATE_EXIT_ACTION

                            STATE_END
                        break;

                    case SERVO_STATE_HOMING_CHECK_FOLDBACK:
                        STATE_ENTRY_ACTION
                        stopwatch_start( &me->timer );

                        STATE_TRANSITION_TEST
                        // Check that the servo is loaded against the endstop in foldback mode
                        // When we enter this state, foldback is likely still heading to setpoint
                        // Check the value is reasonable for a continuous slice of time
                        if( stopwatch_lap( &me->timer ) > SERVO_HOMING_FOLDBACK_CHECK_START_MS )
                        {
                            // Check if foldback holding torque value is outside the expected range...
                            if( servo_feedback > ( -1.0f * SERVO_HOMING_FOLDBACK_TORQUE_MIN )
                                && servo_feedback < ( -1.0f * SERVO_HOMING_FOLDBACK_TORQUE_MAX ) )
                            {

                                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                            }

                            // If we made it this far while being in range, everything looks good
                            if( stopwatch_lap( &me->timer ) < SERVO_HOMING_FOLDBACK_CHECK_END_MS )
                            {
                                STATE_NEXT( SERVO_STATE_HOMING_SUCCESS );
                            }
                        }
                        STATE_EXIT_ACTION

                        STATE_END
                        break;

                    case SERVO_STATE_HOMING_SUCCESS:
                        STATE_ENTRY_ACTION
                        stopwatch_start( &me->timer );
                        me->homing_feedback = 0.0f;
                        STATE_TRANSITION_TEST
                        // Servo successfully found home, and is likely moving to the start-offset point
                        // Wait for the servo to settle in torque domain
                        float feedback_error = ( servo_feedback - me->homing_feedback );
                        me->homing_feedback  = ( 0.1f * servo_feedback ) + ( me->ic_feedback_trim * 0.9f );

                        // Current torque feedback is similar to previous running average
                        if( IS_IN_DEADBAND( feedback_error, 0.0f, SERVO_HOMING_SIMILARITY_PERCENT ) )
                        {
                            // Check we've maintained this level for a minimum amount of time
                            if( stopwatch_lap( &me->timer ) > SERVO_HOMING_SIMILARITY_MS )
                            {
                                // The servo's internal homing procedure can be configured to move to a position after homing
                                // This angle is what this system would interpret that position as
                                float home_angle        = me->config.angle_at_home;
                                me->angle_current_steps = convert_angle_steps( me, home_angle );
                                //                    user_interface_motor_target_angle( servo, home_angle );    // update UI with angles before a target is sent in

                                // Goal position needs to be the current position at init, or a large commanded move would occur
                                me->angle_target_steps = me->angle_current_steps;
                                me->has_high_load      = false;

                                STATE_NEXT( SERVO_STATE_ACTIVE );
                            }
                        }
                        else    // Position is substantially different from previous trend
                        {
                            // Reset the similarity timer
                            stopwatch_start( &me->timer );
                        }

                        // Motor is taking too long to complete the homing process
                        if( stopwatch_lap( &me->timer ) > SERVO_HOMING_COMPLETE_MAX_MS )
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
                            bool   direction_ccw  = ( me->angle_current_steps > me->angle_target_steps ) ^ me->config.reverse_direction;
                            int8_t step_direction = ( direction_ccw ) ? 1 : -1;

                            hal_gpio_write_pin( me->hardware.pin_direction, direction_ccw );

                            // Clamp the number of steps per process loop's 'millisecond tick'
                            uint32_t pulses_needed = MIN( step_difference * step_direction, 8 );

                            for( uint32_t pulses = 0; pulses < pulses_needed; pulses++ )
                            {
                                hal_gpio_toggle_pin( me->hardware.pin_step );
                                // TODO work out how to delay for a short period of time?
                                // TODO: remove shoddy hack using freeRTOS timing
                                vTaskDelay( 1 );
                                //                    hal_delay_us( SERVO_PULSE_DURATION_US );
                                hal_gpio_toggle_pin( me->hardware.pin_step );
                                vTaskDelay( 1 );
                                //                    hal_delay_us( SERVO_PULSE_DURATION_US );

                                me->angle_current_steps = me->angle_current_steps + ( step_direction * -1 );
                            }

                            // Track movement requests over time for velocity estimate
                            //                average_short_update( &me->step_statistics, pulses_needed );

                            // Reset the 'no-motion' idle timer
                            stopwatch_deadline_start( &me->timer, SERVO_IDLE_SETTLE_MS );
                        }
                        else    // no movement needed
                        {
                            //                average_short_update( &me->step_statistics, 0 );

                            // Has the servo been idle for longer than the settling period?
                            if( stopwatch_deadline_elapsed( &me->timer ) )
                            {
                                // Are the power or torque values high for a no-movement load?
                                if( servo_power > SERVO_IDLE_POWER_ALERT_W
                                    || !IS_IN_DEADBAND( servo_feedback, 0, SERVO_IDLE_TORQUE_ALERT ) )
                                {
                                    me->has_high_load = true;
                                }
                            }
                        }

                        if( hal_gpio_read_pin( me->hardware.pin_oc_fault ) == false )
                        {
                            STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                        }
                        STATE_EXIT_ACTION

                        STATE_END
                        break;
                }

                //    user_interface_motor_state( servo, me->currentState );
                //    user_interface_motor_enable( servo, me->enabled );
                //    user_interface_motor_feedback( servo, servo_feedback );
                //    user_interface_motor_power( servo, servo_power );
                //    user_interface_motor_speed( servo, servo_get_degrees_per_second(servo) );

                xSemaphoreGive( xClearpathMutex );
            }
        } // end activation semaphore
    }  // end task loop
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
convert_angle_steps( Servo_t *me, float angle )
{
    REQUIRE( me );
    REQUIRE( me->config.ratio > 0.0f + FLT_EPSILON );

    float steps_per_servo_degree = (float)me->config.steps_per_revolution / SERVO_ANGLE_PER_REV;
    float output_ratio = me->config.ratio;

    // TODO remove/work out why angle_min is involved here at all?
    float servo_angle = ( angle + me->config.angle_min );

    float converted_angle  = servo_angle * output_ratio * steps_per_servo_degree;
    return (int32_t)converted_angle;
}

/* -------------------------------------------------------------------------- */

/*
 * Convert a motor position in steps to an angle in the motor reference frame
 */
PRIVATE float
convert_steps_angle( Servo_t *me, int32_t steps )
{
    REQUIRE( me );
    REQUIRE( me->config.ratio > 0.0f + FLT_EPSILON );

    float steps_per_degree = (float)me->config.steps_per_revolution / SERVO_ANGLE_PER_REV;
    float output_ratio = me->config.ratio;

    float steps_to_angle = (float)steps / steps_per_degree / output_ratio;
    return steps_to_angle;
}

/* -------------------------------------------------------------------------- */

PRIVATE void servo_load_configuration( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    switch( servo )
    {
        case _CLEARPATH_1:
        case _CLEARPATH_2:
        case _CLEARPATH_3:
            me->config.installed            = true;
            me->config.requires_homing      = true;
            me->config.reverse_direction    = false;
            me->config.steps_per_revolution = SERVO_STEPS_PER_REV;
            me->config.ratio                = 1.0f;
            me->config.angle_min            = -1.0f*SERVO_MIN_ANGLE;
            me->config.angle_max            = SERVO_MAX_ANGLE;
            me->config.angle_at_home        = -42.0f;
        break;

        case _CLEARPATH_4:
            me->config.installed            = false;
            me->config.requires_homing      = false;
            me->config.reverse_direction    = false;
            me->config.steps_per_revolution = 0;
            me->config.ratio                = 0;
            me->config.angle_min            = 0;
            me->config.angle_max            = 0;
            me->config.angle_at_home        = 0;
        break;

        default:
            ASSERT(false);
    }
}

PRIVATE void servo_load_hardware( ClearpathServoInstance_t servo)
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    switch( servo )
    {
        case _CLEARPATH_1:
             me->hardware.pin_enable     = _SERVO_1_ENABLE;
             me->hardware.pin_direction  = _SERVO_1_A;
             me->hardware.pin_step       = _SERVO_1_B;
             me->hardware.sensor_current = SENSOR_SERVO_1_CURRENT;
             me->hardware.sensor_hlfb    = SENSOR_SERVO_1_HLFB;
             me->hardware.pin_oc_fault   = _SERVO_1_CURRENT_FAULT;
            break;

        case _CLEARPATH_2:
            me->hardware.pin_enable     = _SERVO_2_ENABLE;
            me->hardware.pin_direction  = _SERVO_2_A;
            me->hardware.pin_step       = _SERVO_2_B;
            me->hardware.sensor_current = SENSOR_SERVO_2_CURRENT;
            me->hardware.sensor_hlfb    = SENSOR_SERVO_2_HLFB;
            me->hardware.pin_oc_fault   = _SERVO_2_CURRENT_FAULT;
            break;

        case _CLEARPATH_3:
            me->hardware.pin_enable     = _SERVO_3_ENABLE;
            me->hardware.pin_direction  = _SERVO_3_A;
            me->hardware.pin_step       = _SERVO_3_B;
            me->hardware.sensor_current = SENSOR_SERVO_3_CURRENT;
            me->hardware.sensor_hlfb    = SENSOR_SERVO_3_HLFB;
            me->hardware.pin_oc_fault   = _SERVO_3_CURRENT_FAULT;
            break;

        case _CLEARPATH_4:
            me->hardware.pin_enable     = _SERVO_4_ENABLE;
            me->hardware.pin_direction  = _SERVO_4_A;
            me->hardware.pin_step       = _SERVO_4_B;
            me->hardware.sensor_current = SENSOR_SERVO_4_CURRENT;
            me->hardware.sensor_hlfb    = SENSOR_SERVO_4_HLFB;
            me->hardware.pin_oc_fault   = _SERVO_4_CURRENT_FAULT;
            break;

        default:
            ASSERT(false);
    }
}

/* ----- End ---------------------------------------------------------------- */