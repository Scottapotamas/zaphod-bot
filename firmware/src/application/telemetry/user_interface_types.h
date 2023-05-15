#ifndef USER_INTERFACE_TYPES_H
#define USER_INTERFACE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "movement_types.h"

/* -------------------------------------------------------------------------- */

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
    // microcontroller info
    uint8_t cpu_load;     // percentage
    uint8_t cpu_clock;    // speed in Mhz

    int16_t input_voltage;    // voltage
    int16_t temp_pcb_ambient;
    int16_t temp_pcb_regulator;
    int16_t temp_external_probe;
    int16_t temp_cpu;

} SystemData_t;

typedef struct
{
    uint8_t supervisor;
    uint8_t motors;
    uint8_t control_mode;
    uint8_t queue_movements;
    uint8_t queue_lighting;
} SystemStates_t;

/* -------------------------------------------------------------------------- */

typedef struct
{
    // Dimensions used in the IK/FK calculations
    uint32_t shoulder_radius_micron;
    uint32_t bicep_length_micron;
    uint32_t forearm_length_micron;
    uint32_t effector_radius_micron;

    // Limits in cartesian space
    int32_t limit_radius;
    int32_t limit_z_min;
    int32_t limit_z_max;

    // Flags if an axis is inverted
    int8_t flip_x;
    int8_t flip_y;
    int8_t flip_z;

} KinematicsInfo_t;

typedef struct
{
    uint16_t speed_rpm;
    uint8_t  setpoint_percentage;
    uint8_t  state;
} FanData_t;

typedef struct
{
    CartesianPoint_t position;    // global position of end effector in cartesian space
    uint32_t speed;               // microns/second
} EffectorData_t;

typedef struct
{
    int16_t feedback;       // in % * 10 of max torque
    int16_t power;          // in watts * 10
    int16_t target_angle;   // in degrees * 100
    int16_t speed;          // in degrees per second * 10
    uint8_t state;          // enum state value
    uint8_t obsolete;       // TODO remove
} MotorData_t;

/* -------------------------------------------------------------------------- */

typedef struct
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint8_t  enable;
} LedState_t;

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif    // USER_INTERFACE_TYPES_H
