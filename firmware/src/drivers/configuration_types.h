#ifndef CONFIGURATION_TYPES_H
#define CONFIGURATION_TYPES_H

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

// TODO: work out how to alias/copy the FanCurve_t from fan
//       without making all of fan.h available downstream
#include "fan.h"

/* ----- Defines ------------------------------------------------------------ */

typedef struct
{
    unsigned buzzer_mute : 1;
    unsigned effector_as_status_led : 1;
    unsigned pendant_optional : 1;
    unsigned pendant_verify_on_arm : 1;
    unsigned pendant_light_enabled : 1;
    unsigned inverted : 1;
    unsigned boundary_violation_mode : 2;
    unsigned reserved: 8;
    unsigned reserved_b2: 8;
    unsigned reserved_b3: 8;
} SettingsBitfieldRenameMe_t;

typedef struct
{
    uint8_t z_rotation;
    uint8_t speed_limit;
    uint8_t volume_x;
    uint8_t volume_y;
    uint8_t volume_z;
    uint8_t reserved_v1;
    uint8_t reserved_v2;
    uint8_t reserved_v3;
} SettingsValuesRenameMe_t;

typedef struct
{
    SettingsBitfieldRenameMe_t flags;
    SettingsValuesRenameMe_t values;
} UserConfig_t;

/* -------------------------------------------------------------------------- */

typedef struct
{
    int16_t voltage;
    int16_t current_servo_1;
    int16_t current_servo_2;
    int16_t current_servo_3;
    int16_t current_servo_4;
} PowerCalibration_t;

typedef struct
{
    bool     correct_gamma;
    bool     correct_wb;
    uint16_t balance_red;
    uint16_t balance_green;
    uint16_t balance_blue;
    uint16_t balance_total;
} LedSettings_t;

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */

#endif    // CONFIGURATION_TYPES_H
