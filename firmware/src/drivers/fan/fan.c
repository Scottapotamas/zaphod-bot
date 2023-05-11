/* -------------------------------------------------------------------------- */

#include <string.h>

#include "fan.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "simple_state_machine.h"
#include "qassert.h"
#include "hal_pwm.h"
#include "signals.h"
/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

/* -------------------------------------------------------------------------- */

#define FAN_MIN_SETPOINT 5U
#define FAN_STALL_FAULT_RPM 140U
#define FAN_STALL_MAX_RPM 1800U
#define NUM_FAN_CURVE_POINTS 5U
#define FAN_FREQUENCY_HZ 20000U
#define FAN_STARTUP_TIME_MS 1000U
#define FAN_STALL_WAIT_TIME_MS 3000U

/* -------------------------------------------------------------------------- */

typedef enum {
    FAN_SENSOR_NONE = 0,
    FAN_SENSOR_TEMPERATURE,
    FAN_SENSOR_SPEED,
} FAN_SENSOR_TYPE;

typedef struct
{
    FAN_SENSOR_TYPE type;
    float value;
} FanInput_t;

typedef struct
{
    FanState_t previousState;
    FanState_t currentState;
    FanState_t nextState;
    QueueHandle_t xRequestQueue;
    uint8_t    speed;            // as a percentage 0-100, what the fan should be at 'now'
} Fan_t;

FanCurve_t default_curve[NUM_FAN_CURVE_POINTS] = {
    { .temperature = 0, .percentage = 20 },
    { .temperature = 20, .percentage = 20 },
    { .temperature = 35, .percentage = 45 },
    { .temperature = 45, .percentage = 90 },
    { .temperature = 60, .percentage = 100 },
};

FanCurve_t user_curve[NUM_FAN_CURVE_POINTS] = { 0 };

PRIVATE Fan_t fan;
PRIVATE FanCurve_t *fan_curve;

PRIVATE Observer sensor_observer = { 0 };

/* -------------------------------------------------------------------------- */

PRIVATE uint8_t fan_speed_at_temp( float temperature );
PRIVATE void fan_sensors_callback(ObserverEvent_t event, EventData eData, void *context);

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_init( void )
{
    memset( &fan, 0, sizeof( fan ) );
    fan_curve = (FanCurve_t *)&default_curve;

    Fan_t *me = &fan;

    // Setup an internal use queue
    // TODO: remove as the observer events can be used with a wake (binary semph?)
    me->xRequestQueue = xQueueCreate( 10, sizeof(FanInput_t) );
    REQUIRE( me->xRequestQueue );
    vQueueAddToRegistry( me->xRequestQueue, "fanInputs");  // Debug view annotation

    hal_pwm_generation( _PWM_TIM_FAN, FAN_FREQUENCY_HZ );

    // Subscribe to the sensor events needed for fan control
    observer_init( &sensor_observer, fan_sensors_callback, NULL );
    observer_subscribe( &sensor_observer, SENSOR_FAN_SPEED );
    observer_subscribe( &sensor_observer, SENSOR_TEMPERATURE_EXTERNAL );
}

/* -------------------------------------------------------------------------- */

void fan_sensors_callback(ObserverEvent_t event, EventData eData, void *context)
{
    Fan_t *me = &fan;
    FanInput_t new = { 0 };

    switch( event )
    {
        case SENSOR_FAN_SPEED:
            new.type = FAN_SENSOR_SPEED;
            new.value = eData.stamped.data.f32;
            xQueueSendToBack( me->xRequestQueue, (void *)&new, 0 );
        break;

        case SENSOR_TEMPERATURE_EXTERNAL:
            new.type = FAN_SENSOR_TEMPERATURE;
            new.value = eData.stamped.data.f32;
            xQueueSendToBack( me->xRequestQueue, (void *)&new, 0 );
            break;

        default:
            // Why did we recieve a signal that we didn't subscribe to?
            ASSERT(false);
            break;
    }
}


PUBLIC Observer * fan_get_observer( void )
{
    return &sensor_observer;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_set_curve( FanCurve_t *curve, uint8_t num_points )
{
    if( !curve || num_points > NUM_FAN_CURVE_POINTS )
    {
        fan_curve = (FanCurve_t *)&default_curve;
    }
    else
    {
        // Copy the points into the user curve bank (with some basic range handling
        for( uint8_t point = 0; point <= num_points; point++ )
        {
            user_curve[point].temperature = CLAMP(curve->temperature, 0, 100);
            user_curve[point].percentage = CLAMP(curve->percentage, 0, 100);
        }

        fan_curve = (FanCurve_t *)&user_curve;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC FanState_t
fan_get_state( void )
{
    Fan_t *me = &fan;

    return me->currentState;
}

/* -------------------------------------------------------------------------- */

PUBLIC void fan_task( void *arg )
{
    Fan_t *me = &fan;

    for(;;)
    {
        // Block until new sensor data arrives
        // If nothing arrives for 500ms, run (and we'll assume something's gone wrong)
        FanInput_t new_data = { 0 };
        xQueueReceive( me->xRequestQueue, &new_data, pdMS_TO_TICKS(500) );

        switch( new_data.type )
        {
            case FAN_SENSOR_NONE:
                // Should only hit this after a 'no sensor data' timeout...
                // So go to full speed
                me->speed = 100;
                break;

            case FAN_SENSOR_TEMPERATURE:
                // Calculate target speed based on temperature curve
                me->speed = fan_speed_at_temp( new_data.value );
                break;

            case FAN_SENSOR_SPEED:
                // Rotor stop detection
                if(    me->currentState == FAN_STATE_ON
                    && new_data.value < FAN_STALL_FAULT_RPM )
                {
                    STATE_NEXT( FAN_STATE_STALL );
                }
                break;
        }

        // Manage the fan's startup/stop behaviour
        switch( me->currentState )
        {
            case FAN_STATE_OFF:
                STATE_ENTRY_ACTION
                    // Set a zero duty cycle - fan needs to support 0-duty spin down though
                    me->speed = 0;
                    hal_pwm_set_percentage_f( _PWM_TIM_FAN, me->speed );
                STATE_TRANSITION_TEST
                    // Trigger startup blip
                    if( me->speed > FAN_MIN_SETPOINT )
                    {
                        STATE_NEXT( FAN_STATE_START );
                    }
                STATE_EXIT_ACTION
                STATE_END
                break;

            case FAN_STATE_STALL:
                STATE_ENTRY_ACTION
                    // Stop the fan because we think it's stalled
                    me->speed = 0;
                    hal_pwm_set_percentage_f( _PWM_TIM_FAN, me->speed );
                STATE_TRANSITION_TEST
                    vTaskDelay( pdMS_TO_TICKS(FAN_STALL_WAIT_TIME_MS) );
                    STATE_NEXT( FAN_STATE_START );
                STATE_EXIT_ACTION
                STATE_END
                break;

            case FAN_STATE_START:
                STATE_ENTRY_ACTION
                    // Set to 100% for a short period
                    me->speed = 100;
                    hal_pwm_set_percentage_f( _PWM_TIM_FAN, me->speed );
                STATE_TRANSITION_TEST
                    vTaskDelay( pdMS_TO_TICKS(FAN_STARTUP_TIME_MS) );
                    STATE_NEXT( FAN_STATE_ON );
                STATE_EXIT_ACTION
                STATE_END
                break;

            case FAN_STATE_ON:
                STATE_ENTRY_ACTION
                STATE_TRANSITION_TEST
                    hal_pwm_set_percentage_f( _PWM_TIM_FAN, CLAMP( me->speed, 0.0f, 100.0f ) );

                    if( me->speed < FAN_MIN_SETPOINT )
                    {
                        STATE_NEXT( FAN_STATE_OFF );
                    }
                STATE_EXIT_ACTION
                    me->speed = 0;
                STATE_END
                break;
        }
    }
}

/* -------------------------------------------------------------------------- */

// Calculate an appropriate fan speed percentage based on temperature lookup table.

PRIVATE uint8_t
fan_speed_at_temp( float temperature )
{
    REQUIRE( fan_curve );

    // Protect against out-of-bounds temperature inputs
    if( (uint8_t)temperature < fan_curve[0].temperature )
    {
        // Temperature is lower than the lowest point in LUT
        return fan_curve[0].percentage;
    }
    else if( (uint8_t)temperature > fan_curve[NUM_FAN_CURVE_POINTS - 1].temperature )
    {
        // Temperature exceeds max LUT value
        return 100.0f;
    }

    for( uint32_t i = 0; i < NUM_FAN_CURVE_POINTS - 1; i++ )
    {
        // Within range between two rows of the LUT
        if( (uint8_t)temperature > fan_curve[i].temperature && (uint8_t)temperature <= fan_curve[i + 1].temperature )
        {
            // Linear interpolation for fan speed between the surrounding rows in LUT
            uint8_t value = MAP( temperature,
                               fan_curve[i].temperature,
                               fan_curve[i + 1].temperature,
                               fan_curve[i].percentage,
                               fan_curve[i + 1].percentage );

            return value;
        }
    }

    return 97;    // Should have returned with a valid percentage by now, fail ON for safety.
}

/* -------------------------------------------------------------------------- */
