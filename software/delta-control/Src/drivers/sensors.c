/* ----- Local Includes ----------------------------------------------------- */

#include "sensors.h"
#include "hal_adc.h"
#include "hal_temperature.h"
#include "hal_power.h"
#include "hal_systick.h"

/* ----- Public Functions --------------------------------------------------- */

/* Init the hardware for the board sensors */

PUBLIC void
sensors_init( void )
{
	sensors_enable();	//todo move somewhere more suitable
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
}

/* -------------------------------------------------------------------------- */

/* Return averaged and converted temperature readings in degrees C */

PUBLIC float
sensors_ambient_C( void )
{
	return hal_temperature_pcb_degrees_C( hal_adc_read_avg( HAL_ADC_INPUT_TEMP_PCB ) );
}

PUBLIC float
sensors_12v_regulator_C( void )
{
	return hal_temperature_pcb_degrees_C( hal_adc_read_avg( HAL_ADC_INPUT_TEMP_REG ) );

}

PUBLIC float
sensors_expansion_C( void )
{
	return hal_temperature_ext_degrees_C( hal_adc_read_avg( HAL_ADC_INPUT_TEMP_EXT ) );

}

PUBLIC float
sensors_microcontroller_C( void )
{
	return hal_temperature_micro_degrees_C( hal_adc_read_avg( HAL_ADC_INPUT_TEMP_INTERNAL ) );
}

/* -------------------------------------------------------------------------- */

/* Return averaged and converted power measurements */

PUBLIC float
sensors_input_V( void )
{
	return hal_voltage_V( hal_adc_read_avg( HAL_ADC_INPUT_VOLT_SENSE ) );
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

/* ----- End ---------------------------------------------------------------- */
