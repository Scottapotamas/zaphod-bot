#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "configuration_types.h"

#include "app_times.h"
#include "app_version.h"
#include "buzzer.h"
#include "hal_flashmem.h"

/* ----- Defines ------------------------------------------------------------ */

PowerCalibration_t power_trims        = { 0 };
LedSettings_t      led_calibration    = { 0 };
UserConfig_t       user_configuration = { 0 };
FanCurve_t         fan_curve[NUM_FAN_CURVE_POINTS] = { 0 };

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
configuration_init( void )
{
    // perform any setup here if needed
    configuration_set_defaults();

    // Load settings from flash memory
    //    configuration_load();
}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_set_defaults( void )
{
    led_calibration.correct_gamma = false;
    led_calibration.correct_wb    = true;
    led_calibration.balance_red   = 0xFFFFU * 0.3f;
    led_calibration.balance_green = 0;
    led_calibration.balance_blue  = 0xFFFF * 0.79f;

    user_configuration.values.z_rotation = 0;
    user_configuration.values.speed_limit = EFFECTOR_SPEED_LIMIT / 10;

}

/* -------------------------------------------------------------------------- */

#define PERSIST_ID_CAL_POWER 1
#define PERSIST_ID_CAL_LED   2
#define PERSIST_ID_FAN_CURVE 3
#define PERSIST_ID_CONFIG 4

PUBLIC void
configuration_load( void )
{
    // Load the data from non-volatile storage
    hal_flashmem_retrieve( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
    hal_flashmem_retrieve( PERSIST_ID_CAL_LED, &led_calibration, sizeof( LedSettings_t ) );
    hal_flashmem_retrieve( PERSIST_ID_FAN_CURVE, &fan_curve, sizeof( fan_curve ) );
    hal_flashmem_retrieve( PERSIST_ID_CONFIG, &user_configuration, sizeof( UserConfig_t ) );

}

/* -------------------------------------------------------------------------- */

PUBLIC void
configuration_save( void )
{
    // save settings to memory
    hal_flashmem_store( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
    hal_flashmem_store( PERSIST_ID_CAL_LED, &led_calibration, sizeof( LedSettings_t ) );
    hal_flashmem_store( PERSIST_ID_FAN_CURVE, &fan_curve, sizeof( fan_curve ) );
    hal_flashmem_store( PERSIST_ID_CONFIG, &user_configuration, sizeof( UserConfig_t ) );

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
    return &fan_curve[0];
}

PUBLIC void
configuration_notify_fan_curve( void )
{
    fan_set_curve( &fan_curve[0], DIM(fan_curve) );
}

/* -------------------------------------------------------------------------- */

PUBLIC UserConfig_t *
configuration_get_user_config_ptr( void )
{
    return &user_configuration;
}

PUBLIC void
configuration_notify_config( void )
{
    // Notify relevant modules of new configuration
    buzzer_mute( user_configuration.flags.buzzer_mute );


    // TODO: consider doing damage tracking to only set relevant members?


}

PUBLIC uint16_t
configuration_get_effector_speed_limit( void )
{
    return user_configuration.values.speed_limit * 10;
}

PUBLIC float
configuration_get_z_rotation( void )
{
    return (float)user_configuration.values.z_rotation * 2.0f;
}

/* -------------------------------------------------------------------------- */

PUBLIC PowerCalibration_t *
configuration_get_power_calibration_ptr( void )
{
    return &power_trims;
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

PUBLIC LedSettings_t *
configuration_get_led_calibration_ptr( void )
{
    return &led_calibration;
}

PUBLIC void
configuration_get_led_whitebalance( uint16_t *red_offset, uint16_t *green_offset, uint16_t *blue_offset )
{
    *red_offset   = led_calibration.balance_red;
    *green_offset = led_calibration.balance_green;
    *blue_offset  = led_calibration.balance_blue;
}

PUBLIC void
configuration_get_led_bias( uint16_t *offset )
{
    *offset = led_calibration.balance_total;
}

PUBLIC bool
configuration_get_led_luma_correction_enabled( void )
{
    return led_calibration.correct_gamma;
}

PUBLIC bool
configuration_get_led_wb_correction_enabled( void )
{
    return led_calibration.correct_wb;
}

/* -------------------------------------------------------------------------- */



/* ----- End ---------------------------------------------------------------- */
