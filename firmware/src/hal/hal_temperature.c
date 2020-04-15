/* ----- System Includes ---------------------------------------------------- */

#include <math.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "hal_temperature.h"

/* -------------------------------------------------------------------------- */

/* Temperature Calibration Values
 * See STM32F4xx datasheet "Temperature sensor characteristics"
 * */

#define V25       0.76f   /* V */
#define AVG_SLOPE 0.0025f /* 2.5mV/C */

PUBLIC float
hal_temperature_micro_degrees_C( uint32_t raw_adc )
{
    if( raw_adc == 0 )
    {
        return -1000.0;
    }

    float Vsense = ( (float)raw_adc * 3.3f ) / 4096.0f;

    return ( ( ( Vsense - V25 ) / AVG_SLOPE ) + 25.0f );
}

/* -------------------------------------------------------------------------- */

// See https://sparks.gogo.co.nz/ntc_thermistor.html for calculator spreadsheet

/* Thermistor Linearisation and conversion
 * Converts adc reading from onboard 10K thermistors to temperature in deg C.
 *
 * Thermistor(s) on PCB is a 0603 SMD Murata NCP18XH103J03RB
 * Nominal B-Constant 3350-3399K (3360 used in calc)
 * 10K resistance at 25C, 5% tolerance
 * Assumes 2.4k low side resistor, thermistor fed from AVCC
 *
 * Conversion designed for 0-120�C operation
 * */

PUBLIC float
hal_temperature_pcb_degrees_C( uint32_t raw_adc )
{
    if( raw_adc == 0 )
    {
        return -1000.0;
    }

	return ( 1/( (log(((2700.0 * (4096.0 - raw_adc)) / raw_adc)/10000.0)/3360.0) + (1 / (273.15 + 25.000)) ) ) - 273.15;
}

/* -------------------------------------------------------------------------- */

/*
 * Thermistor on 2-pin header is a random aliexpress 10k NTC,
 * https://www.aliexpress.com/item/10K-Precision-Epoxy-Thermistor-3435-NTC-Pack-of-2/32819250168.html
 *
 * Nominal B-Constant 3435K
 * 10K resistance at 25C, '1% tolerance' claimed
 * Assumes 2.4k low side resistor, thermistor fed from AVCC
 *
 * Conversion designed for 0-120�C operation
 * */

PUBLIC float
hal_temperature_ext_degrees_C( uint32_t raw_adc )
{
    if( raw_adc == 0 )
    {
        return -1000.0;
    }

    return ( 1 / ( ( log( ( ( 2400.0 * ( 4096.0 - raw_adc ) ) / raw_adc ) / 10000.0 ) / 3435.0 ) + ( 1 / ( 273.15 + 25.000 ) ) ) ) - 273.15;
}

/* ----- End ---------------------------------------------------------------- */
