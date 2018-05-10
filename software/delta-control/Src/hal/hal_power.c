/* ----- Local Includes ----------------------------------------------------- */

#include "hal_power.h"

/* -------------------------------------------------------------------------- */

PUBLIC float
hal_voltage_V( uint32_t raw_adc )
{
    float Vsense = ( (float)raw_adc * 3.3f ) / 4096.0f;

    //Resistor divider 260K over 10K = 27:1 ratio
    return ( Vsense * ( 260.0f + 10.0f ) / 10.0f );
}

/* -------------------------------------------------------------------------- */

PUBLIC float
hal_current_A( uint32_t raw_adc )
{
    float Vsense = ( (float)raw_adc * 3.3f ) / 4096.0f;

    //Current sense voltage is +-90mV/+-A, where 0A is at VCC/2
    //Calc sense IC output voltage (remove 0A offset bias), convert to amps
    return ( Vsense - ( 3.3f / 2.0f ) ) / 0.090f;
}

/* ----- End ---------------------------------------------------------------- */
