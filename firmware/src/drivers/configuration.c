#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "configuration_types.h"

#include "hal_flashmem.h"
#include "app_version.h"
#include "app_times.h"
#include "buzzer.h"

/* ----- Defines ------------------------------------------------------------ */

PowerCalibration_t power_trims = { 0 };
LedSettings_t *rgb_led_settings_ptr = 0;

float z_rotation = 0;

FanCurve_t fan_curve[NUM_FAN_CURVE_POINTS] = {
        { .temperature = 0, .percentage = 20 },
        { .temperature = 20, .percentage = 20 },
        { .temperature = 35, .percentage = 45 },
        { .temperature = 45, .percentage = 90 },
        { .temperature = 60, .percentage = 100 },
};

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
configuration_init( void )
{
    //perform any setup here if needed
    configuration_set_defaults();

    // Load settings from flash memory
    //    configuration_load();
}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_set_defaults( void )
{
    rgb_led_settings_ptr->correct_gamma = false;
    rgb_led_settings_ptr->correct_wb = true;
    rgb_led_settings_ptr->balance_red = 0xFFFFU * 0.3f;
    rgb_led_settings_ptr->balance_green = 0;
    rgb_led_settings_ptr->balance_blue = 0xFFFF * 0.79f;

}

/* -------------------------------------------------------------------------- */

#define PERSIST_ID_CAL_POWER 1
#define PERSIST_ID_CAL_LED   2
#define PERSIST_ID_FAN_CURVE 3

PUBLIC void
configuration_load( void )
{
    // Load the data from non-volatile storage
    hal_flashmem_retrieve( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
    hal_flashmem_retrieve( PERSIST_ID_CAL_LED, rgb_led_settings_ptr, sizeof( LedSettings_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_save( void )
{
    //save settings to memory
    hal_flashmem_store( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
    hal_flashmem_store( PERSIST_ID_CAL_LED, rgb_led_settings_ptr, sizeof( LedSettings_t ) );

    buzzer_sound( 2, 4000, 50 );
}

/* -------------------------------------------------------------------------- */

PRIVATE void configuration_wipe( void )
{
    hal_flashmem_wipe_and_prepare();
    buzzer_sound( 1, 1000, 100 );
}

/* -------------------------------------------------------------------------- */

PUBLIC FanCurve_t *
configuration_get_fan_curve_ptr( void )
{
    if( DIM( fan_curve ) == NUM_FAN_CURVE_POINTS )
    {
        return &fan_curve[0];
    }

    return 0;
}

PUBLIC int16_t configuration_get_voltage_trim_mV( void )
{
    return power_trims.voltage;
}

PUBLIC int16_t configuration_get_servo_trim_mA( uint8_t servo )
{
    switch( servo )
    {
        case 0:    // Servo 1
            return power_trims.current_servo_1;
        case 1:    // Servo 2
            return power_trims.current_servo_2;
        case 2:    // Servo 3
            return power_trims.current_servo_3;
        case 3:    // Servo 4
            return power_trims.current_servo_4;
        default:
            return 0;
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void *
configuration_set_led_correction_ptr( LedSettings_t *ptr )
{
    if(ptr)
    {
        rgb_led_settings_ptr = ptr;
    }

    return 0;
}

PUBLIC void
configuration_get_led_whitebalance( uint16_t *red_offset, uint16_t *green_offset, uint16_t *blue_offset )
{
    *red_offset   = rgb_led_settings_ptr->balance_red;
    *green_offset = rgb_led_settings_ptr->balance_green;
    *blue_offset  = rgb_led_settings_ptr->balance_blue;
}

PUBLIC void
configuration_get_led_bias( uint16_t *offset )
{
    *offset = rgb_led_settings_ptr->balance_total;
}

PUBLIC bool
configuration_get_led_luma_correction_enabled( void )
{
    return rgb_led_settings_ptr->correct_gamma;
}

PUBLIC bool
configuration_get_led_wb_correction_enabled( void )
{
    return rgb_led_settings_ptr->correct_wb;
}

/* -------------------------------------------------------------------------- */

PUBLIC float
configuration_get_rotation_z()
{
    float angle = CLAMP( z_rotation, 0.0f, 360.0f );
    return angle;
}


/* ----- End ---------------------------------------------------------------- */
