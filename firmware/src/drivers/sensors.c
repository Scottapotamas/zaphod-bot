/* ----- Local Includes ----------------------------------------------------- */

#include "sensors.h"
#include "app_times.h"
#include "hal_adc.h"
#include "hal_hard_ic.h"
#include "hal_temperature.h"
#include "hal_power.h"
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
	hal_adc_start( HAL_ADC_INPUT_M1_CURRENT, 	ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_M2_CURRENT, 	ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_M3_CURRENT, 	ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_M4_CURRENT, 	ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_VOLT_SENSE, 	ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_TEMP_PCB, 		ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_TEMP_REG, 		ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_TEMP_EXT, 		ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_TEMP_INTERNAL, ADC_SAMPLE_RATE_MS );
	hal_adc_start( HAL_ADC_INPUT_VREFINT, 		ADC_SAMPLE_RATE_MS );

    config_set_sensors_enabled(true);
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
	hal_adc_stop( HAL_ADC_INPUT_TEMP_PCB 		    );
	hal_adc_stop( HAL_ADC_INPUT_TEMP_REG 		    );
	hal_adc_stop( HAL_ADC_INPUT_TEMP_EXT 		    );
	hal_adc_stop( HAL_ADC_INPUT_TEMP_INTERNAL 	);
	hal_adc_stop( HAL_ADC_INPUT_VREFINT			);

    config_set_sensors_enabled(false);
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
	input_voltage += (float)config_get_voltage_trim_mV()/1000;

    config_set_input_voltage( input_voltage );
	return input_voltage;
}

PUBLIC float
sensors_servo_A( HalAdcInput_t servo_to_sample )
{
	if( servo_to_sample < HAL_ADC_INPUT_M1_CURRENT || servo_to_sample > HAL_ADC_INPUT_M4_CURRENT )
	{
		return -1000.0f;
	}

	float measured_current      = hal_current_A( hal_adc_read_avg( servo_to_sample ) );
	float calibration_offset    = (float)config_get_servo_trim_mA(servo_to_sample)/1000;

	return measured_current + calibration_offset;
}

PUBLIC float
sensors_servo_W( HalAdcInput_t servo_to_sample )
{
	if( servo_to_sample < HAL_ADC_INPUT_M1_CURRENT || servo_to_sample > HAL_ADC_INPUT_M4_CURRENT )
	{
		return -1000.0f;
	}

	return sensors_input_V() * sensors_servo_A( servo_to_sample );
}

PUBLIC uint16_t
sensors_fan_speed_RPM( void )
{
    // 1hz = 60rpm
	//two hall pulses per revolution

	// TODO read higher res hall ic value for better RPM calculation
	float hall_frequency = hal_hard_ic_read(HAL_HARD_IC_FAN_HALL );
	uint16_t rpm = 60 * (hall_frequency / 2);
    config_set_fan_rpm( rpm );
	return rpm;
}

/* ----- End ---------------------------------------------------------------- */
