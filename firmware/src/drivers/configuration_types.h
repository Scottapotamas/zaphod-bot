#ifndef CONFIGURATION_TYPES_H
#define CONFIGURATION_TYPES_H

/* ----- Defines ------------------------------------------------------------ */



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
    int16_t balance_red;
    int16_t balance_green;
    int16_t balance_blue;
    int16_t balance_total;
} LedSettings_t;

/* -------------------------------------------------------------------------- */

typedef struct
{
    uint8_t temperature;
    uint8_t percentage;
} FanCurve_t;

/* -------------------------------------------------------------------------- */

#endif //CONFIGURATION_TYPES_H
