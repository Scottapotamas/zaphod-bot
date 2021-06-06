#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "configuration_types.h"

#include "hal_flashmem.h"
#include "app_version.h"
#include "buzzer.h"

/* ----- Defines ------------------------------------------------------------ */

BuildInfo_t      fw_info;
PowerCalibration_t power_trims;
LedSettings_t rgb_led_settings;

float z_rotation = 0;

FanCurve_t fan_curve[] = {
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
    //set build info to hardcoded values
    memset( &fw_info.build_branch, 0, sizeof( fw_info.build_branch ) );
    memset( &fw_info.build_info, 0, sizeof( fw_info.build_info ) );
    memset( &fw_info.build_date, 0, sizeof( fw_info.build_date ) );
    memset( &fw_info.build_time, 0, sizeof( fw_info.build_time ) );
    memset( &fw_info.build_type, 0, sizeof( fw_info.build_type ) );
    memset( &fw_info.build_name, 0, sizeof( fw_info.build_name ) );

    strcpy( (char *)&fw_info.build_branch, ProgramBuildBranch );
    strcpy( (char *)&fw_info.build_info, ProgramBuildInfo );
    strcpy( (char *)&fw_info.build_date, ProgramBuildDate );
    strcpy( (char *)&fw_info.build_time, ProgramBuildTime );
    strcpy( (char *)&fw_info.build_type, ProgramBuildType );
    strcpy( (char *)&fw_info.build_name, ProgramName );
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
    hal_flashmem_retrieve( PERSIST_ID_CAL_LED, &rgb_led_settings, sizeof( LedSettings_t ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_save( void )
{
    //save settings to memory
    hal_flashmem_store( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
    hal_flashmem_store( PERSIST_ID_CAL_LED, &rgb_led_settings, sizeof( LedSettings_t ) );

    buzzer_sound( 2, 4000, 50 );
}

/* -------------------------------------------------------------------------- */

PRIVATE void configuration_wipe( void )
{
    hal_flashmem_wipe_and_prepare();
    buzzer_sound( 1, 1000, 100 );
}

/* -------------------------------------------------------------------------- */

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

PUBLIC void
configuration_get_led_whitebalance( int16_t *red_offset, int16_t *green_offset, int16_t *blue_offset )
{
    *red_offset   = rgb_led_settings.balance_red;
    *green_offset = rgb_led_settings.balance_green;
    *blue_offset  = rgb_led_settings.balance_blue;
}

PUBLIC void
configuration_get_led_bias( int16_t *offset )
{
    *offset = rgb_led_settings.balance_total;
}

/* -------------------------------------------------------------------------- */

PUBLIC float
configuration_get_rotation_z()
{
    float angle = CLAMP( z_rotation, 0.0f, 360.0f );
    return angle;
}


/* ----- End ---------------------------------------------------------------- */
