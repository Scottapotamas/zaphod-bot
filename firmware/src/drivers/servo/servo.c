/* -------------------------------------------------------------------------- */

#include <string.h>

#include "global.h"
#include "qassert.h"
#include "simple_state_machine.h"

#include "servo.h"
#include "servo_settings.h"
#include "servo_hardware.h"

#include "signals.h"
#include "broker.h"
#include "stopwatch.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

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

/* -------------------------------------------------------------------------- */

typedef struct
{
    ServoState_t previousState;
    ServoState_t currentState;
    ServoState_t nextState;

    ClearpathServoInstance_t identifier;
    bool       enabled;

    float      ic_feedback_trim;    // calculated prior to arming
    float      homing_feedback;     // calculated after homing
    stopwatch_t   timer;

    int32_t    angle_current_steps;
    int32_t    angle_target_steps;

    TimerHandle_t servo_stats_timer;
    uint32_t angle_update_timestamp;

    SemaphoreHandle_t mutex;
    Subscriber *event_sub;      // subscribe to system events etc

    float power;                // power consumption in watts
    float hlfb;                 // feedback value from servo
    stopwatch_t hlfb_stopwatch; // timestamp of most recent hlfb update
    bool       presence_detected;
} Servo_t;

/* -------------------------------------------------------------------------- */

PRIVATE void servo_event_handler( Servo_t *me );

PRIVATE void servo_operation( Servo_t *me );

PRIVATE void servo_set_target_angle_limited( ClearpathServoInstance_t servo, float angle_degrees );

PRIVATE void servo_publish_angle( ClearpathServoInstance_t servo );

PRIVATE void servo_publish_velocity( ClearpathServoInstance_t servo, int32_t steps_since_last );

PRIVATE void servo_statistics_stale( TimerHandle_t xTimer );

/* -------------------------------------------------------------------------- */

PRIVATE Servo_t clearpath[_NUMBER_CLEARPATH_SERVOS];

/* -------------------------------------------------------------------------- */

PUBLIC void
servo_init( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    memset( me, 0, sizeof( Servo_t ) );
    servo_load_configuration( servo );
    servo_load_hardware_defaults( servo );

    me->identifier = servo;

    if( !me->mutex )
    {
        me->mutex = xSemaphoreCreateMutex();
    }
    ENSURE( me->mutex );

    // Setup sensor event subscriptions
    switch( me->identifier )
    {
        case _CLEARPATH_1:
            me->event_sub = broker_create_subscriber( "psCP1", 10 );
            broker_add_event_subscription( me->event_sub, SERVO_1_POWER );
            broker_add_event_subscription( me->event_sub, SENSOR_SERVO_1_HLFB );
            broker_add_event_subscription( me->event_sub, SERVO_TARGET_DEGREES );
            break;
        case _CLEARPATH_2:
            me->event_sub = broker_create_subscriber( "psCP2", 10 );
            broker_add_event_subscription( me->event_sub, SERVO_2_POWER );
            broker_add_event_subscription( me->event_sub, SENSOR_SERVO_2_HLFB );
            broker_add_event_subscription( me->event_sub, SERVO_TARGET_DEGREES );
            break;
        case _CLEARPATH_3:
            me->event_sub = broker_create_subscriber( "psCP3", 10 );
            broker_add_event_subscription( me->event_sub, SERVO_3_POWER );
            broker_add_event_subscription( me->event_sub, SENSOR_SERVO_3_HLFB );
            broker_add_event_subscription( me->event_sub, SERVO_TARGET_DEGREES );
            break;
        case _CLEARPATH_4:
            me->event_sub = broker_create_subscriber( "psCP4", 10 );
            broker_add_event_subscription( me->event_sub, SERVO_4_POWER );
            broker_add_event_subscription( me->event_sub, SENSOR_SERVO_4_HLFB );

            // TODO: expansion motor needs to subscribe to a target angle topic
            break;

        default:
            ASSERT_PRINTF(false, "Init an invalid servo");
    }
    ENSURE( me->event_sub );

    // Subscribe to 'unicast to servo' topics
    broker_add_event_subscription( me->event_sub, OVERWATCH_SERVO_ENABLE );
    broker_add_event_subscription( me->event_sub, OVERWATCH_SERVO_DISABLE );

    // Used for servo statistics publishing
    me->servo_stats_timer = xTimerCreate("servoStatsTimer",
                                        pdMS_TO_TICKS(10),  // TODO this timeout should be set correctly
                                        pdFALSE,
                                        (void *)servo,
                                        servo_statistics_stale
    );
    REQUIRE( me->servo_stats_timer );
    xTimerStart( me->servo_stats_timer, pdMS_TO_TICKS(10) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void* servo_get_state_context_for( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    return (void*)&clearpath[servo];
}

/* -------------------------------------------------------------------------- */

PUBLIC void servo_task( void* arg )
{
    Servo_t *me = arg;
    ENSURE( me );

    PublishedEvent new_data = { 0 };

    // Early exit if not configured
    if( !servo_is_configured(me->identifier) )
    {
        // TODO: allow servo configuration to occur dynamically?
        return;
    }

    for(;;)
    {
        // Block until a command or new sensor data arrives
        xQueuePeek( me->event_sub->queue, &new_data, portMAX_DELAY );

        // Get access to the servo's data
        if( xSemaphoreTake( me->mutex, pdMS_TO_TICKS(1) ) )
        {
            // Handle pending queue events
            servo_event_handler( me );

            // Check if the HLFB signals arrived recently as proxy for 'servo detection'
            me->presence_detected = !stopwatch_deadline_elapsed( &me->hlfb_stopwatch );

            // Servo feedback loss in any active state should trigger immediate error handling behaviour
            if( !me->presence_detected && me->enabled )
            {
                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
            }

            // If disabled (by supervisor etc) then immediately start disable process
            if( ( !me->enabled )
                && ( me->currentState > SERVO_STATE_ERROR_RECOVERY ) )
            {
                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
            }

            // Run the state-machine
            servo_operation( me );

            if( STATE_IS_TRANSITIONING )
            {
                PublishedEvent state_update = { 0 };
                state_update.topic = SERVO_1_STATE + me->identifier;
                state_update.data.stamped.value.u32 = me->nextState;
                broker_publish( &state_update );
            }

            xSemaphoreGive( me->mutex );
        }
        else // didn't get the mutex fast enough
        {
            ASSERT_PRINTF(false, "Servo mutex contested");
        }
    }  // end task loop
}

/* -------------------------------------------------------------------------- */

PRIVATE void servo_event_handler( Servo_t *me )
{
    PublishedEvent new_data = { 0 };
    bool event_ready = xQueueReceive( me->event_sub->queue, &new_data, 0 );

    // Handle all pending events in the queue
    while( event_ready )
    {
        switch( new_data.topic )
        {
            case SERVO_1_POWER:
            case SERVO_2_POWER:
            case SERVO_3_POWER:
            case SERVO_4_POWER:
                me->power = new_data.data.stamped.value.f32;
                break;

            case SENSOR_SERVO_1_HLFB:
            case SENSOR_SERVO_2_HLFB:
            case SENSOR_SERVO_3_HLFB:
            case SENSOR_SERVO_4_HLFB:
                me->hlfb = new_data.data.stamped.value.f32;

                // Refresh the 'recent hlfb seen' stopwatch
                // Instead of using the normal stopwatch function which uses current time
                //     stopwatch_deadline_start( &me->hlfb_timestamp, SERVO_MISSING_HLFB_MS );
                // use the timestamp value attached to the HLFB reading
                me->hlfb_stopwatch = new_data.data.stamped.timestamp + SERVO_MISSING_HLFB_MS;
                break;

            case OVERWATCH_SERVO_ENABLE:
                me->enabled = true;
                break;

            case OVERWATCH_SERVO_DISABLE:
                me->enabled = false;
                break;

            case SERVO_TARGET_DEGREES:
                servo_set_target_angle_limited( me->identifier,
                                                new_data.data.f_triple[me->identifier] );
                break;

            default:
                // Why did we receive a signal that we didn't subscribe to?
                ASSERT(false);
                break;
        }

        // Check if more events are pending, so we can process those as well
        event_ready = xQueueReceive( me->event_sub->queue, &new_data, 0 );
    }
}

PRIVATE void servo_operation( Servo_t *me )
{
    float servo_feedback = me->hlfb - me->ic_feedback_trim;

    switch( me->currentState )
    {
        case SERVO_STATE_INACTIVE:
            STATE_ENTRY_ACTION
            servo_hw_disable( me->identifier );
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
            servo_hw_disable( me->identifier );

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
            me->ic_feedback_trim = me->hlfb;
            stopwatch_deadline_start( &me->timer, SERVO_HOMING_CALIBRATION_MS );
            STATE_TRANSITION_TEST

            if( !stopwatch_deadline_elapsed( &me->timer ) )
            {
                // The trim value is a super simple FIR of the incoming values
                const float leaky_integrator_factor = 0.6f;
                me->ic_feedback_trim = (leaky_integrator_factor * me->ic_feedback_trim) + ( 1.0f - leaky_integrator_factor )*me->hlfb;
            }
            else
            {
                // Check that the corrected feedback value with our trim is pretty close to zero
                if( IS_IN_DEADBAND( servo_feedback, 0.0f, 0.5f ) )
                {
                    if( servo_config_requires_homing(me->identifier) )
                    {
                        STATE_NEXT( SERVO_STATE_HOMING_FIND_ENDSTOP );
                    }
                    else
                    {
                        // Just enable the servo then 'torque settle' validation
                        servo_hw_enable( me->identifier );
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
            servo_hw_enable( me->identifier );
            stopwatch_deadline_start( &me->timer, SERVO_HOMING_MAX_MS );
            STATE_TRANSITION_TEST
            // We expect the torque to be positive while the arm transits towards the endstop
            // When it goes under 0% HLFB, the torque has changed direction which indicates likely endstop contact
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
            me->homing_feedback  = ( 0.1f * servo_feedback ) + ( me->homing_feedback * 0.9f );

            // Current torque feedback is similar to previous running average
            if( IS_IN_DEADBAND( feedback_error, 0.0f, SERVO_HOMING_SIMILARITY_PERCENT ) )
            {
                // Check we've maintained this level for a minimum amount of time
                if( stopwatch_lap( &me->timer ) > SERVO_HOMING_SIMILARITY_MS )
                {
                    // The servo's internal homing procedure can be configured to move to a position after homing
                    // This angle is what this system would interpret that position as
                    float home_angle        = servo_config_get_home_angle(me->identifier);
                    me->angle_current_steps = servo_angle_to_steps( me->identifier, home_angle );

                    // Goal position needs to be the current position at init, or a large commanded move would occur
                    me->angle_target_steps = me->angle_current_steps;

                    STATE_NEXT( SERVO_STATE_ACTIVE );
                }
            }
            else    // Feedback is substantially different from previous trend
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
                bool   direction_ccw  = ( me->angle_current_steps > me->angle_target_steps ) ^ servo_config_direction_reversed(me->identifier);
                int8_t step_direction = ( direction_ccw ) ? 1 : -1;

                // Clamp the number of steps per process loop's 'millisecond tick'
                uint16_t pulses_needed = MIN( step_difference * step_direction, SERVO_MAX_STEPS_PER_TICK );

                servo_hw_step( me->identifier, direction_ccw, pulses_needed);
                me->angle_current_steps = me->angle_current_steps + ( pulses_needed * step_direction * -1);

                servo_publish_angle( me->identifier );

                // Track movement requests over time for velocity estimate
                servo_publish_velocity( me->identifier, step_difference );

                // Reset the 'no-motion' idle timer
                stopwatch_deadline_start( &me->timer, SERVO_IDLE_SETTLE_MS );
            }
            else    // no movement needed
            {
                // Has the servo been idle for longer than the settling period?
                if( stopwatch_deadline_elapsed( &me->timer ) )
                {
                    // Are the power or torque values high for a no-movement load?
                    if( me->power > SERVO_IDLE_POWER_ALERT_W
                        || !IS_IN_DEADBAND( servo_feedback, 0, SERVO_IDLE_TORQUE_ALERT ) )
                    {
                        STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
                    }
                }
            }

            // TODO: Move this above state-machine?
            if( servo_hw_oc_fault(me->identifier) )
            {
                STATE_NEXT( SERVO_STATE_ERROR_RECOVERY );
            }
            STATE_EXIT_ACTION

            STATE_END
            break;
    }

}

/* -------------------------------------------------------------------------- */

// Calculates and sets target position, constrains input to legal angles only
PRIVATE void servo_set_target_angle_limited( ClearpathServoInstance_t servo, float angle_degrees )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    if( servo_angle_within_limits( me->identifier, angle_degrees ) )
    {
        me->angle_target_steps = servo_angle_to_steps( me->identifier, angle_degrees );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC float servo_get_current_angle( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    return servo_steps_to_angle( me->identifier, me->angle_current_steps );
}

/* -------------------------------------------------------------------------- */

PRIVATE void servo_publish_angle( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    PublishedEvent angle_update = { 0 };
    angle_update.topic = SERVO_1_POSITION + me->identifier;
    angle_update.data.stamped.value.f32 = servo_steps_to_angle( me->identifier, me->angle_current_steps );
    angle_update.data.stamped.timestamp = xTaskGetTickCount();
    broker_publish( &angle_update );
}

/* -------------------------------------------------------------------------- */

PRIVATE void servo_publish_velocity( ClearpathServoInstance_t servo, int32_t steps_since_last )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    // Difference between positions over time
    uint32_t timestamp_now = xTaskGetTickCount();
    uint32_t delta_time = timestamp_now - me->angle_update_timestamp;

    // TODO: how should we handle sub millisecond moves?
    if( !delta_time )
    {
        // For now, assume a zero duration is one millisecond
        delta_time += 1;
//        ENSURE( delta_time );
    }

    // Publish the velocity value
    PublishedEvent vel_update = { 0 };
    vel_update.topic = SERVO_1_SPEED + me->identifier;
    vel_update.data.stamped.timestamp = timestamp_now;
    vel_update.data.stamped.value.f32 = servo_steps_to_angle( me->identifier, steps_since_last ) / (float)delta_time * 1000.0f;
    broker_publish( &vel_update );

    // Refresh the stats stale timer
    if( steps_since_last )
    {
        xTimerReset( me->servo_stats_timer, pdMS_TO_TICKS(2) );
    }

    me->angle_update_timestamp = timestamp_now;
}

/* -------------------------------------------------------------------------- */

// Called when the effector task sits waiting for input events
// We assume no update events within the timeout period mean the velocity should be zero
PRIVATE void servo_statistics_stale( TimerHandle_t xTimer )
{
    ClearpathServoInstance_t servo = (ClearpathServoInstance_t)pvTimerGetTimerID(xTimer);
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    Servo_t *me = &clearpath[servo];

    if( xSemaphoreTake(me->mutex, portMAX_DELAY) )
    {
        // Calculate and publish the velocity using a zero-distance update
        servo_publish_velocity( me->identifier, 0 );

        xSemaphoreGive(me->mutex);
    }
}

/* -------------------------------------------------------------------------- */