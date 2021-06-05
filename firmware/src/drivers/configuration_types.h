#ifndef CONFIGURATION_TYPES_H
#define CONFIGURATION_TYPES_H

/* ----- Defines ------------------------------------------------------------ */

typedef struct
{
    char build_branch[12];
    char build_info[12];
    char build_date[12];
    char build_time[12];
    char build_type[12];
    char build_name[12];
} BuildInfo_t;

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

#endif //CONFIGURATION_TYPES_H
