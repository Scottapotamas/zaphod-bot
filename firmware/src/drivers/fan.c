/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "app_times.h"
#include "configuration.h"
#include "fan.h"
#include "hal_pwm.h"
#include "sensors.h"
#include "simple_state_machine.h"
#include "timer_ms.h"

/* ----- Private Types ------------------------------------------------------ */

typedef struct
{
    FanState_t previousState;
    FanState_t currentState;
    FanState_t nextState;
    uint8_t    speed;            // as a percentage 0-100, what the fan should be at 'now'
    uint8_t    set_speed;        // as a percentage 0-100, requested speed target
    timer_ms_t startup_timer;    // amount of time to 'blip' the fan for reliable starts
} Fan_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Fan_t fan;

PRIVATE FanCurve_t *fan_curve;

PRIVATE uint8_t
fan_speed_at_temp( float temperature );

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
fan_init( void )
{
    memset( &fan, 0, sizeof( fan ) );

    // Get a pointer to the fan curve configuration table
    fan_curve = configuration_get_fan_curve_ptr();
    hal_pwm_generation( _PWM_TIM_FAN, FAN_FREQUENCY_HZ );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_set( uint8_t speed_percentage )
{
    Fan_t *me = &fan;

    // 0-100% speed control over fan
    me->set_speed = CLAMP( speed_percentage, 0, 100 );
}

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
fan_get_speed( void )
{
    Fan_t *me = &fan;

    return me->speed;
}

/* -------------------------------------------------------------------------- */

PUBLIC uint16_t
fan_get_rpm( void )
{
    // TODO consider moving fan speed calculation into this file?
    return sensors_fan_speed_RPM();
}

/* -------------------------------------------------------------------------- */

PUBLIC FanState_t
fan_get_state( void )
{
    Fan_t *me = &fan;

    return me->currentState;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_process( void )
{
    Fan_t *me = &fan;

    // Get the current fan speed
    uint16_t fan_hall_rpm = sensors_fan_speed_RPM();

    switch( me->currentState )
    {
        case FAN_STATE_OFF:
            STATE_ENTRY_ACTION

            // Make sure fan is not spinning
            me->speed = 0;

            STATE_TRANSITION_TEST

            me->set_speed = fan_speed_at_temp( sensors_expansion_C() );

            // Once new target is established, trigger startup blip
            if( me->set_speed > 0 )
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
            timer_ms_start( &me->startup_timer, FAN_STALL_WAIT_TIME_MS );
            STATE_TRANSITION_TEST
            // Check if the current setpoint is likely under the RPM stall limit
            if( me->set_speed < ( FAN_STALL_MAX_RPM / FAN_STALL_FAULT_RPM ) )
            {
                me->set_speed += 10;    // increase the setpoint by 10%
            }

            if( timer_ms_is_expired( &me->startup_timer ) )
            {
                STATE_NEXT( FAN_STATE_START );
            }
            STATE_EXIT_ACTION
            STATE_END
            break;

        case FAN_STATE_START:
            STATE_ENTRY_ACTION
            // Set PWM to 100% for configurable short period
            me->speed = 100;
            timer_ms_start( &me->startup_timer, FAN_STARTUP_TIME_MS );
            STATE_TRANSITION_TEST
            if( timer_ms_is_expired( &me->startup_timer ) )
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
            // Calculate target speed based on temperature reading
            me->set_speed = fan_speed_at_temp( sensors_expansion_C() );

            // Speed change req while running
            if( me->set_speed != me->speed )
            {
                me->speed = me->set_speed;
            }

            // Fan req turned off
            if( me->set_speed == 0 )
            {
                STATE_NEXT( FAN_STATE_OFF );
            }

            // Rotor stop detection
            if( fan_hall_rpm < FAN_STALL_FAULT_RPM )
            {
                // restart the fan
                STATE_NEXT( FAN_STATE_STALL );
            }

            STATE_EXIT_ACTION

            STATE_END
            break;
    }

    hal_pwm_set_percentage_f( _PWM_TIM_FAN, me->speed );
}

/* -------------------------------------------------------------------------- */

// Calculate an appropriate fan speed percentage based on temperature lookup table.

PRIVATE uint8_t
fan_speed_at_temp( float temperature )
{
    // Ensure the pointer is set (or default to a high fan speed)
    if( fan_curve )
    {
        // Protect against out-of-bounds temperature inputs
        if( (uint8_t)temperature < fan_curve[0].temperature )
        {
            // Temperature is lower than lowest point in LUT
            return fan_curve[0].percentage;
        }
        else if( (uint8_t)temperature > fan_curve[NUM_FAN_CURVE_POINTS - 1].temperature )
        {
            // Temperature exceeds max LUT value
            return 100.0;
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
    }

    return 97;    // Should have returned with a valid percentage by now, fail ON for safety.
}

/* ----- End ---------------------------------------------------------------- */
