/* -------------------------------------------------------------------------- */

#include <string.h>

#include "configuration.h"
#include "configuration_types.h"
#include "qassert.h"

//#include "hal_flashmem.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;   // used for qassert.h

/* -------------------------------------------------------------------------- */

PowerCalibration_t power_trims        = { 0 };
LedSettings_t      led_calibration    = { 0 };
UserConfig_t       user_configuration = { 0 };
//FanCurve_t         fan_curve[NUM_FAN_CURVE_POINTS] = { 0 };

typedef enum
{
    KV_BOOL = 0,
    KV_CHAR,
    KV_INT8,
    KV_UINT8,
    KV_INT16,
    KV_UINT16,
    KV_INT32,
    KV_UINT32,
    KV_FLOAT,
} ValueType_t;

typedef struct
{
    ValueType_t type;
    union
    {
        bool     bl;
        char     ch;
        uint8_t  u8;
        int8_t   i8;
        uint16_t u16;
        int16_t  i16;
        uint32_t u32;
        int32_t  i32;
        float    f32;
    } data;
} KeyValueEntry_t;

const KeyValueEntry_t config_template[NUM_ENTRIES] = {
    [BUZZER_MUTED] = { .type = KV_BOOL, .data.bl = false },

    [KINEMATICS_SPEED_LIMIT]   = { .type = KV_UINT32, .data.u32 = 650U },
    [KINEMATICS_Z_ROTATE]      = { .type = KV_FLOAT, .data.f32 = 0.0f },
    [KINEMATICS_VOLUME_RADIUS] = { .type = KV_UINT32, .data.u32 = 0 },
    [KINEMATICS_VOLUME_HEIGHT] = { .type = KV_UINT32, .data.u32 = 0 },

    [EXPANSION_ENABLED]         = { .type = KV_BOOL, .data.bl = false },
    [EXPANSION_TYPE]            = { .type = KV_UINT8, .data.u8 = 0 },
    [EXPANSION_FEEDBACK]        = { .type = KV_BOOL, .data.bl = false },
    [EXPANSION_REQUIRES_HOMING] = { .type = KV_BOOL, .data.bl = false },
    [EXPANSION_REVERSE]         = { .type = KV_BOOL, .data.bl = false },
    [EXPANSION_RESOLUTION]      = { .type = KV_BOOL, .data.bl = false },
    [EXPANSION_RATIO]           = { .type = KV_FLOAT, .data.f32 = 0.0f },
    [EXPANSION_SPEED_LIMIT]     = { .type = KV_UINT32, .data.u32 = 0 },
    [EXPANSION_RANGE_MIN]       = { .type = KV_INT32, .data.i32 = 0 },
    [EXPANSION_RANGE_MAX]       = { .type = KV_INT32, .data.i32 = 0 },

    [CALIBRATION_VOLTAGE_OFFSET]  = { .type = KV_INT32, .data.i32 = -3650 },
    [CALIBRATION_CURRENT_SERVO_1] = { .type = KV_INT32, .data.i32 = -25 },
    [CALIBRATION_CURRENT_SERVO_2] = { .type = KV_INT32, .data.i32 = 45 },
    [CALIBRATION_CURRENT_SERVO_3] = { .type = KV_INT32, .data.i32 = -205 },
    [CALIBRATION_CURRENT_SERVO_4] = { .type = KV_INT32, .data.i32 = -195 },

    [WHITEBALANCE_RED]   = { .type = KV_UINT16, .data.u16 = (uint16_t)( 0xFFFFU * 0 ) },
    [WHITEBALANCE_GREEN] = { .type = KV_UINT16, .data.u16 = (uint16_t)( 0xFFFF * 0.25f ) },
    [WHITEBALANCE_BLUE]  = { .type = KV_UINT16, .data.u16 = (uint16_t)( 0xFFFF * 0.86f ) },
};

KeyValueEntry_t config_store[NUM_ENTRIES];

/* -------------------------------------------------------------------------- */

PUBLIC void configuration_init( void )
{
    // perform any setup here if needed
    configuration_set_defaults();

    // Load settings from flash memory
    configuration_load();
}

/* -------------------------------------------------------------------------- */

PUBLIC void configuration_set_defaults( void )
{
    memcpy( config_store, config_template, sizeof(config_store) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void configuration_load( void )
{
    // Load the data from non-volatile storage
//    hal_flashmem_retrieve( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
//    hal_flashmem_retrieve( PERSIST_ID_CAL_LED, &led_calibration, sizeof( LedSettings_t ) );
//    hal_flashmem_retrieve( PERSIST_ID_FAN_CURVE, &fan_curve, sizeof( fan_curve ) );
//    hal_flashmem_retrieve( PERSIST_ID_CONFIG, &user_configuration, sizeof( UserConfig_t ) );

}

/* -------------------------------------------------------------------------- */

PUBLIC void configuration_save( void )
{
    // save settings to memory
//    hal_flashmem_store( PERSIST_ID_CAL_POWER, &power_trims, sizeof( PowerCalibration_t ) );
//    hal_flashmem_store( PERSIST_ID_CAL_LED, &led_calibration, sizeof( LedSettings_t ) );
//    hal_flashmem_store( PERSIST_ID_FAN_CURVE, &fan_curve, sizeof( fan_curve ) );
//    hal_flashmem_store( PERSIST_ID_CONFIG, &user_configuration, sizeof( UserConfig_t ) );

}

/* -------------------------------------------------------------------------- */

PRIVATE void configuration_wipe( void )
{
//    hal_flashmem_wipe_and_prepare();

}

/* -------------------------------------------------------------------------- */

PUBLIC bool configuration_get_bool( ConfigurationEntry_t id )
{
    REQUIRE( id < NUM_ENTRIES );
    REQUIRE( config_store[id].type == KV_BOOL );

    return config_store[id].data.bl;
}

PUBLIC void configuration_set_bool( ConfigurationEntry_t id, bool value )
{
    REQUIRE( id < NUM_ENTRIES );
    REQUIRE( config_store[id].type == KV_BOOL );

    config_store[id].data.bl = value;
}

/* -------------------------------------------------------------------------- */

PUBLIC uint32_t configuration_get_uint32( ConfigurationEntry_t id )
{
    REQUIRE( id < NUM_ENTRIES );
    REQUIRE( config_store[id].type == KV_BOOL );

    return config_store[id].data.u32;
}

PUBLIC void configuration_set_uint32( ConfigurationEntry_t id, uint32_t value )
{
    REQUIRE( id < NUM_ENTRIES );
    REQUIRE( config_store[id].type == KV_BOOL );

    config_store[id].data.u32 = value;
}

/* -------------------------------------------------------------------------- */

PUBLIC int32_t configuration_get_int32_t( ConfigurationEntry_t id )
{
    REQUIRE( id < NUM_ENTRIES );
    REQUIRE( config_store[id].type == KV_BOOL );

    return config_store[id].data.i32;
}

PUBLIC void configuration_set_int32_t( ConfigurationEntry_t id, int32_t value )
{
    REQUIRE( id < NUM_ENTRIES );
    REQUIRE( config_store[id].type == KV_BOOL );

    config_store[id].data.i32 = value;
}

/* -------------------------------------------------------------------------- */

PUBLIC float configuration_get_float( ConfigurationEntry_t id )
{
    REQUIRE( id < NUM_ENTRIES );
    REQUIRE( config_store[id].type == KV_BOOL );

    return config_store[id].data.f32;
}

PUBLIC void configuration_set_float( ConfigurationEntry_t id, float value )
{
    REQUIRE( id < NUM_ENTRIES );
    REQUIRE( config_store[id].type == KV_BOOL );

    config_store[id].data.f32 = value;
}

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

PUBLIC void configuration_notify_config( void )
{
    // Notify relevant modules of new configuration
//    buzzer_mute( user_configuration.flags.buzzer_mute );

//    servo_change_configuration( _CLEARPATH_4,
//                                user_configuration.flags.expansion_enabled,
//                                user_configuration.flags.expansion_requires_homing,
//                                user_configuration.flags.expansion_reverse,
//                                user_configuration.values.expansion_resolution * 100,
//                                user_configuration.values.expansion_ratio,                   // TODO: remap into float
//                                user_configuration.values.expansion_range_min,
//                                user_configuration.values.expansion_range_max,
//                                0                                                     // TODO: position/angle at home value?
//                                );

    // TODO: consider doing damage tracking to only set relevant members?


}

/* -------------------------------------------------------------------------- */