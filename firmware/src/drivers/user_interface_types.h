#ifndef USER_INTERFACE_TYPES_H
#define USER_INTERFACE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

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

typedef enum
{
    CONTROL_NONE = 0,
    CONTROL_MANUAL,
    CONTROL_EVENT,
    CONTROL_TRACK,
    CONTROL_DEMO,
    CONTROL_CHANGING,
} ControlModes_t;

typedef struct
{
    bool sensors_enable;    //if ADC sampling and conversions are active
    bool module_enable;     //add-in card powered

    //microcontroller info
    uint8_t cpu_load;     //percentage
    uint8_t cpu_clock;    //speed in Mhz

    float input_voltage;    //voltage
} SystemData_t;

typedef struct
{
    uint8_t supervisor;
    uint8_t motors;
    uint8_t control_mode;
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
    float pcb_ambient;
    float pcb_regulator;
    float external_probe;
    float cpu_temp;
} TempData_t;

typedef struct
{
    //pathing engine state idle, running, etc
    uint8_t pathing_state;
    //motion handler information
    uint8_t motion_state;
    //information about the current move being executed
    uint8_t  profile_type;
    uint8_t  move_progress;
    uint16_t movement_identifier;
} MotionData_t;

typedef struct
{
    uint8_t movements;
    uint8_t lighting;
} QueueDepths_t;

typedef struct
{
    uint8_t enabled;
    uint8_t state;
    int16_t feedback;
    float   target_angle;
    float   power;
} MotorData_t;

/* -------------------------------------------------------------------------- */

typedef struct
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint8_t  enable;
} LedState_t;

typedef struct
{
    float   hue;
    float   saturation;
    float   lightness;
    uint8_t enable;
} LedControl_t;

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif //USER_INTERFACE_TYPES_H
