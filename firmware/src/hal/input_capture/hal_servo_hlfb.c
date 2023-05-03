/* -------------------------------------------------------------------------- */

#include "hal_servo_hlfb.h"

/* -------------------------------------------------------------------------- */

PUBLIC float hal_servo_hlfb( uint32_t raw_counts )
{
    // HLFB from servos is a square-wave where 5% < x < 95% is used for torque/speed output
    // i.e. 65% DC => 1/3rd max torque in +ve direction
    // The hardware input capture driver returns the value as %duty cycle

    // Get the HLFB duty, and scale to -100% to +100% range
    float percentage = (raw_counts / 100.0f ) * 2.05f - 100.0f;
    percentage = CLAMP( percentage, -100.0f, 100.0f );

    return percentage;
}

/* -------------------------------------------------------------------------- */