/* -------------------------------------------------------------------------- */

#include <string.h>

#include "fan.h"

#include "FreeRTOS.h"
#include "task.h"
#include "simple_state_machine.h"
#include "qassert.h"

#include "hal_pwm.h"

// TODO input rpm data via queue/task notification/subscription instead of raw calls
#include "hal_ic_hard.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE; /* Used for ASSERT checks to define __FILE__ only once */

#define FAN_STALL_FAULT_RPM 140U
#define FAN_STALL_MAX_RPM 1800U
#define NUM_FAN_CURVE_POINTS 5U
#define FAN_FREQUENCY_HZ 20000U
#define FAN_STARTUP_TIME_MS 1000U
#define FAN_STALL_WAIT_TIME_MS 3000U

/* -------------------------------------------------------------------------- */

typedef struct
{
    FanState_t previousState;
    FanState_t currentState;
    FanState_t nextState;
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

/* -------------------------------------------------------------------------- */

PRIVATE uint8_t fan_speed_at_temp( float temperature );
PRIVATE void fan_process( void *arg );

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_init( void )
{
    memset( &fan, 0, sizeof( fan ) );
    fan_curve = (FanCurve_t *)&default_curve;

    xTaskCreate(fan_process, "fan", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

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

PRIVATE void
fan_process( void *arg )
{
    Fan_t *me = &fan;

    // TODO this should be done upstream
    hal_ic_hard_init();

    hal_pwm_generation( _PWM_TIM_FAN, FAN_FREQUENCY_HZ );

    for(;;)
    {
        // Get the current fan speed
        //    uint16_t fan_hall_rpm = sensors_fan_speed_RPM();

        // TODO: block on new RPM or temperature data coming in
        //      - after a long timeout go to 100% ?

        float fan_hall_rpm   = hal_ic_hard_read_f(HAL_IC_HARD_FAN_HALL) * 60;
        float expansion_temp = 22.0f;

        switch( me->currentState )
        {
            case FAN_STATE_OFF:
                STATE_ENTRY_ACTION
                    // Set a zero duty cycle - up to the fan to actually spin down
                    hal_pwm_set_percentage_f( _PWM_TIM_FAN, 0.0f );
                STATE_TRANSITION_TEST
                    // If temperature requires it, trigger startup blip
                    if( fan_speed_at_temp( expansion_temp ) )
                    {
                        STATE_NEXT( FAN_STATE_START );
                    }
                STATE_EXIT_ACTION
                STATE_END
                break;

            case FAN_STATE_STALL:
                STATE_ENTRY_ACTION
                    // Stop the fan because we think it's stalled
                    hal_pwm_set_percentage_f( _PWM_TIM_FAN, 0.0f );
                STATE_TRANSITION_TEST
                    // TODO: Check if repeated stalls are occuring?

                    vTaskDelay( pdMS_TO_TICKS(FAN_STALL_WAIT_TIME_MS) );
                    STATE_NEXT( FAN_STATE_START );
                STATE_EXIT_ACTION
                STATE_END
                break;

            case FAN_STATE_START:
                STATE_ENTRY_ACTION
                    // Set PWM to 100% for a short period
                    hal_pwm_set_percentage_f( _PWM_TIM_FAN, 100.0f );
                STATE_TRANSITION_TEST
                    vTaskDelay( pdMS_TO_TICKS(FAN_STARTUP_TIME_MS) );
                    STATE_NEXT( FAN_STATE_ON );
                STATE_EXIT_ACTION
                STATE_END
                break;

            case FAN_STATE_ON:
                STATE_ENTRY_ACTION

                STATE_TRANSITION_TEST
                    // Calculate target speed based on temperature curve
                    me->speed = fan_speed_at_temp( expansion_temp );

                    hal_pwm_set_percentage_f( _PWM_TIM_FAN, CLAMP( me->speed, 0.0f, 100.0f ) );

                    // Fan req turned off
                    if( me->speed < 5 )
                    {
                        STATE_NEXT( FAN_STATE_OFF );
                    }

                    vTaskDelay( pdMS_TO_TICKS(5) );

                    // Rotor stop detection
                    if( fan_hall_rpm < FAN_STALL_FAULT_RPM )
                    {
                        STATE_NEXT( FAN_STATE_STALL );
                    }
                STATE_EXIT_ACTION
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
            return fan_curve[i].percentage + ( ( ( temperature - fan_curve[i].temperature ) / ( fan_curve[i + 1].temperature - fan_curve[i].temperature ) ) * ( fan_curve[i + 1].percentage - fan_curve[i].percentage ) );
        }
    }

    return 97;    // Should have returned with a valid percentage by now, fail ON for safety.
}

/* -------------------------------------------------------------------------- */
