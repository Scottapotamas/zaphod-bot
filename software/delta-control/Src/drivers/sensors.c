/* ----- Local Includes ----------------------------------------------------- */

#include "sensors.h"
#include "hal_adc.h"
#include "hal_soft_ic.h"
#include "hal_temperature.h"
#include "hal_power.h"
#include "hal_systick.h"
#include "configuration.h"

/* ----- Public Functions --------------------------------------------------- */

/* Init the hardware for the board sensors */

PUBLIC void
sensors_init( void )
{

}

/* -------------------------------------------------------------------------- */

/* Start/Enable board sensors */

PUBLIC void
sensors_enable( void )
{
	hal_adc_start( HAL_ADC_INPUT_M1_CURRENT, 	20 );
	hal_adc_start( HAL_ADC_INPUT_M2_CURRENT, 	20 );
	hal_adc_start( HAL_ADC_INPUT_M3_CURRENT, 	20 );
	hal_adc_start( HAL_ADC_INPUT_M4_CURRENT, 	20 );
	hal_adc_start( HAL_ADC_INPUT_VOLT_SENSE, 	20 );
	hal_adc_start( HAL_ADC_INPUT_TEMP_PCB, 		20 );
	hal_adc_start( HAL_ADC_INPUT_TEMP_REG, 		20 );
	hal_adc_start( HAL_ADC_INPUT_TEMP_EXT, 		20 );
	hal_adc_start( HAL_ADC_INPUT_TEMP_INTERNAL, 20 );
	hal_adc_start( HAL_ADC_INPUT_VREFINT, 		20 );

	hal_soft_ic_start( HAL_SOFT_IC_HALL, 1);

	config_sensors_enable(true);
}

/* -------------------------------------------------------------------------- */

/* Stop/Disable board sensors */

PUBLIC void
sensors_disable( void )
{
	hal_adc_stop( HAL_ADC_INPUT_M1_CURRENT 		);
	hal_adc_stop( HAL_ADC_INPUT_M2_CURRENT 		);
	hal_adc_stop( HAL_ADC_INPUT_M3_CURRENT 		);
	hal_adc_stop( HAL_ADC_INPUT_M4_CURRENT 		);
	hal_adc_stop( HAL_ADC_INPUT_VOLT_SENSE 		);
	hal_adc_stop( HAL_ADC_INPUT_TEMP_PCB 		);
	hal_adc_stop( HAL_ADC_INPUT_TEMP_REG 		);
	hal_adc_stop( HAL_ADC_INPUT_TEMP_EXT 		);
	hal_adc_stop( HAL_ADC_INPUT_TEMP_INTERNAL 	);
	hal_adc_stop( HAL_ADC_INPUT_VREFINT			);

	hal_soft_ic_stop( HAL_SOFT_IC_HALL );

	config_sensors_enable(false);
}

/* -------------------------------------------------------------------------- */

/* Return averaged and converted temperature readings in degrees C */

PUBLIC float
sensors_ambient_C( void )
{
	float pcb_ambient_temp = hal_temperature_pcb_degrees_C( hal_adc_read_avg( HAL_ADC_INPUT_TEMP_PCB ) );
	config_set_temp_ambient( pcb_ambient_temp );
	return pcb_ambient_temp;
}

PUBLIC float
sensors_12v_regulator_C( void )
{
	float pcb_regulator_temp = hal_temperature_pcb_degrees_C( hal_adc_read_avg( HAL_ADC_INPUT_TEMP_REG ) );
	config_set_temp_regulator( pcb_regulator_temp );
	return pcb_regulator_temp;
}

PUBLIC float
sensors_expansion_C( void )
{
	float expansion_temp = hal_temperature_ext_degrees_C( hal_adc_read_avg( HAL_ADC_INPUT_TEMP_EXT ) );
	config_set_temp_external( expansion_temp );
	return expansion_temp;
}

PUBLIC float
sensors_microcontroller_C( void )
{
	float die_temp = hal_temperature_micro_degrees_C( hal_adc_read_avg( HAL_ADC_INPUT_TEMP_INTERNAL ) );
	config_set_cpu_temp( die_temp );
	return die_temp;
}

/* -------------------------------------------------------------------------- */

/* Return averaged and converted power measurements */

PUBLIC float
sensors_input_V( void )
{
	float input_voltage = hal_voltage_V( hal_adc_read_avg( HAL_ADC_INPUT_VOLT_SENSE ) );
	config_set_input_voltage( input_voltage );
	return input_voltage;
}

PUBLIC float
sensors_servo_A( HalAdcInput_t servo_to_sample )
{
	if( servo_to_sample <= HAL_ADC_INPUT_M1_CURRENT && servo_to_sample >= HAL_ADC_INPUT_M4_CURRENT )
	{
		return -1000.0;
	}

	return hal_current_A( hal_adc_read_avg( servo_to_sample ) );
}

PUBLIC float
sensors_servo_W( HalAdcInput_t servo_to_sample )
{
	if( servo_to_sample <= HAL_ADC_INPUT_M1_CURRENT && servo_to_sample >= HAL_ADC_INPUT_M4_CURRENT )
	{
		return -1000.0;
	}

	return sensors_input_V() * sensors_servo_A( servo_to_sample );
}

PUBLIC uint16_t
sensors_fan_speed_RPM( void )
{
	//rpm = 60 * rps, where rps = 1000msec / duration (msec) of rising edge width
	//two hall pulses per revolution
	uint16_t rpm = 60 * ((1000 / 2)/hal_soft_ic_read_avg( HAL_SOFT_IC_HALL ) );
    config_set_fan_rpm( rpm );
	return rpm;
}

/* ----- End ---------------------------------------------------------------- */
