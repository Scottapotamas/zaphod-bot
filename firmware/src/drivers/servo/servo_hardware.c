/* -------------------------------------------------------------------------- */

#include "servo_hardware.h"
#include "qassert.h"

#include "hal_gpio.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

// Clearpath will filter pulses shorter than 1us
// ULN2303 NPN driver has rise time of ~5ns, fall of ~10nsec
#define SERVO_PULSE_DURATION_US (10U)

/* -------------------------------------------------------------------------- */

// Servo IO
typedef struct
{
    HalGpioPortPin_t pin_enable;
    HalGpioPortPin_t pin_direction;
    HalGpioPortPin_t pin_step;
    HalGpioPortPin_t pin_oc_fault;
} ServoHardware_t;

PRIVATE ServoHardware_t servo_io[_NUMBER_CLEARPATH_SERVOS];

/* -------------------------------------------------------------------------- */

PRIVATE void servo_delay_us( uint32_t delay_us ) __attribute__( ( optimize( "-O0" ) ) );

/* -------------------------------------------------------------------------- */

// Immediately disable all servo's enable pins
//   Intended for ASSERT style 'disable the servos' end-of-the-world style use
PUBLIC void servo_hw_disable_all( void )
{
    hal_gpio_write_pin( servo_io[_CLEARPATH_1].pin_enable, false );
    hal_gpio_write_pin( servo_io[_CLEARPATH_2].pin_enable, false );
    hal_gpio_write_pin( servo_io[_CLEARPATH_3].pin_enable, false );
    hal_gpio_write_pin( servo_io[_CLEARPATH_4].pin_enable, false );
}

/* -------------------------------------------------------------------------- */

PUBLIC void servo_hw_enable( ClearpathServoInstance_t servo )
{
    hal_gpio_write_pin( servo_io[servo].pin_enable, true );
    hal_gpio_write_pin( servo_io[servo].pin_step, false );
    hal_gpio_write_pin( servo_io[servo].pin_direction, false );
}

/* -------------------------------------------------------------------------- */

PUBLIC void servo_hw_disable( ClearpathServoInstance_t servo )
{
    hal_gpio_write_pin( servo_io[servo].pin_enable, false );
    hal_gpio_write_pin( servo_io[servo].pin_step, false );
    hal_gpio_write_pin( servo_io[servo].pin_direction, false );
}

/* -------------------------------------------------------------------------- */

PUBLIC bool servo_hw_oc_fault( ClearpathServoInstance_t servo )
{
    // OC Fault pin is logically inverted
    return !hal_gpio_read_pin( servo_io[servo].pin_oc_fault );
}

/* -------------------------------------------------------------------------- */

PUBLIC void servo_hw_step( ClearpathServoInstance_t servo,
                           bool direction,
                           uint32_t steps )
{
    hal_gpio_write_pin( servo_io[servo].pin_direction, direction );

    for( uint32_t pulses = 0; pulses < steps; pulses++ )
    {
        // TODO: remove shoddy blocking timing hack
        hal_gpio_toggle_pin( servo_io[servo].pin_step );
        servo_delay_us( SERVO_PULSE_DURATION_US );
        hal_gpio_toggle_pin( servo_io[servo].pin_step );
        servo_delay_us( SERVO_PULSE_DURATION_US );
    }
}

PRIVATE void servo_delay_us( uint32_t delay_us )
{
    // TODO: DON'T USE THIS CORE DEFINITION, TRANSITION TOWARDS TIMER
    uint32_t SystemCoreClock = 168000000;
    uint32_t loop = delay_us * ( SystemCoreClock / 1000000 ) / 5;

    /* Protect against this loop running when loop == 0 */
    if( loop > 0 )
    {
        while( loop-- ) {};
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void servo_load_hardware_defaults( ClearpathServoInstance_t servo )
{
    REQUIRE( servo < _NUMBER_CLEARPATH_SERVOS );
    ServoHardware_t *me = &servo_io[servo];

    switch( servo )
    {
        case _CLEARPATH_1:
            me->pin_enable     = _SERVO_1_ENABLE;
            me->pin_direction  = _SERVO_1_A;
            me->pin_step       = _SERVO_1_B;
            me->pin_oc_fault   = _SERVO_1_CURRENT_FAULT;
            break;

        case _CLEARPATH_2:
            me->pin_enable     = _SERVO_2_ENABLE;
            me->pin_direction  = _SERVO_2_A;
            me->pin_step       = _SERVO_2_B;
            me->pin_oc_fault   = _SERVO_2_CURRENT_FAULT;
            break;

        case _CLEARPATH_3:
            me->pin_enable     = _SERVO_3_ENABLE;
            me->pin_direction  = _SERVO_3_A;
            me->pin_step       = _SERVO_3_B;
            me->pin_oc_fault   = _SERVO_3_CURRENT_FAULT;
            break;

        case _CLEARPATH_4:
            me->pin_enable     = _SERVO_4_ENABLE;
            me->pin_direction  = _SERVO_4_A;
            me->pin_step       = _SERVO_4_B;
            me->pin_oc_fault   = _SERVO_4_CURRENT_FAULT;
            break;

        default:
            ASSERT(false);
    }
}

/* -------------------------------------------------------------------------- */