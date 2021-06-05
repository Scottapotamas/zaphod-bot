#ifndef USER_INTERFACE_TYPES_H
#define USER_INTERFACE_TYPES_H


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



typedef struct
{
    uint8_t  id;    // index of the task (pseudo priority)
    uint8_t  ready;
    uint8_t  queue_used;
    uint8_t  queue_max;
    uint32_t waiting_max;
    uint32_t burst_max;
    char     name[12];    // human readable taskname set during app_tasks setup
} Task_Info_t;

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

uint8_t fan_manual_setpoint = 0;
uint8_t fan_manual_enable   = 0;

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



#endif //USER_INTERFACE_TYPES_H
