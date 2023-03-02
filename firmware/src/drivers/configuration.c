#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "configuration.h"
#include "configuration_types.h"

#include "buzzer.h"
#include "clearpath.h"

#include "app_version.h"
#include "app_times.h"
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
    led_calibration.balance_red   = 0xFFFFU * 0;
    led_calibration.balance_green = 0xFFFFU * 0.46f;
    led_calibration.balance_blue  = 0xFFFF * 0.59f;

    power_trims.voltage = -3650;
    power_trims.current_servo_1 = -25;
    power_trims.current_servo_2 = 45;
    power_trims.current_servo_3 = -205;
    power_trims.current_servo_4 = -195;

    user_configuration.values.z_rotation = 0;
    user_configuration.values.speed_limit = EFFECTOR_SPEED_LIMIT / 10;
    user_configuration.values.volume_radius = KINEMATICS_RADIUS_MAX_MM;
    user_configuration.values.volume_z = KINEMATICS_Z_MAX_MM;

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

    servo_change_configuration( _CLEARPATH_4,
                                user_configuration.flags.expansion_enabled,
                                user_configuration.flags.expansion_requires_homing,
                                user_configuration.flags.expansion_reverse,
                                user_configuration.values.expansion_resolution * 100,
                                user_configuration.values.expansion_ratio,                   // TODO: remap into float
                                user_configuration.values.expansion_range_min,
                                user_configuration.values.expansion_range_max,
                                0                                                     // TODO: position/angle at home value?
                                );

    // TODO: consider doing damage tracking to only set relevant members?


}

PUBLIC uint16_t
configuration_get_effector_speed_limit( void )
{
    // Don't allow user values greater than the default
    return MIN( user_configuration.values.speed_limit * 10, EFFECTOR_SPEED_LIMIT );
}

PUBLIC float
configuration_get_z_rotation( void )
{
    return (float)user_configuration.values.z_rotation * 2.0f;
}

PUBLIC uint32_t
configuration_get_volume_restriction_radius_mm( void )
{
    return MIN( user_configuration.values.volume_radius, KINEMATICS_RADIUS_MAX_MM );
}

PUBLIC uint32_t
configuration_get_volume_restriction_height_mm( void )
{
    return MIN( user_configuration.values.volume_z, KINEMATICS_Z_MAX_MM );
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

PUBLIC bool
configuration_get_expansion_enabled( void )
{
    return user_configuration.flags.expansion_enabled;
}

PUBLIC uint8_t
configuration_get_expansion_motion_type( void )
{
    return user_configuration.flags.expansion_type;
}

PUBLIC uint8_t
configuration_get_expansion_feedback_mode( void )
{
    return user_configuration.flags.expansion_feedback;
}

PUBLIC bool
configuration_get_expansion_homing_required( void )
{
    return user_configuration.flags.expansion_requires_homing;
}

PUBLIC bool
configuration_get_expansion_reversed( void )
{
    return user_configuration.flags.expansion_reverse;
}

PUBLIC uint16_t
configuration_get_expansion_servo_resolution( void )
{
    return user_configuration.values.expansion_resolution;
}

PUBLIC float
configuration_get_expansion_output_ratio( void )
{
    return user_configuration.values.expansion_ratio;
}

PUBLIC int32_t
configuration_get_expansion_limit_min( void )
{
    // TODO: What precision/ is needed here?
    return user_configuration.values.expansion_range_min;
}

PUBLIC int32_t
configuration_get_expansion_limit_max( void )
{
    // TODO: What precision/ is needed here?
    return user_configuration.values.expansion_range_max;
}

PUBLIC uint16_t
configuration_get_expansion_speed_limit( void )
{
    return MIN( user_configuration.values.expansion_speed_limit * 10, EXPANSION_SPEED_LIMIT );
}

/* ----- End ---------------------------------------------------------------- */
