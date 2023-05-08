#ifndef SIGNALS_H
#define SIGNALS_H

typedef enum _SENSOR_EVENT_FLAG {

    // ADC backed events
    // Don't re-order
    SENSOR_SERVO_1_CURRENT = 0,
    SENSOR_SERVO_2_CURRENT,
    SENSOR_SERVO_3_CURRENT,
    SENSOR_SERVO_4_CURRENT,
    SENSOR_VOLTAGE_INPUT,
    SENSOR_TEMPERATURE_PCB,
    SENSOR_TEMPERATURE_REGULATOR,
    SENSOR_TEMPERATURE_EXTERNAL,
    SENSOR_TEMPERATURE_MICRO,
    SENSOR_VOLTAGE_INTERNAL_REF,

    // Input capture backed events
    // Don't re-order
    SENSOR_SERVO_1_HLFB,
    SENSOR_SERVO_2_HLFB,
    SENSOR_SERVO_3_HLFB,
    SENSOR_SERVO_4_HLFB,
    SENSOR_FAN_SPEED,

    SENSOR_NUM_FIELDS,  // Only marks end of enum range, shouldn't exceed 31 entries
} SENSOR_EVENT_FLAG;

typedef enum _SYSTEM_EVENT_FLAG {

    // Servo info
    SERVO_1_POSITION,
    SERVO_1_SPEED,
    SERVO_1_STATE,

    SERVO_2_POSITION,
    SERVO_2_SPEED,
    SERVO_2_STATE,

    SERVO_3_POSITION,
    SERVO_3_SPEED,
    SERVO_3_STATE,

    SERVO_4_POSITION,
    SERVO_4_SPEED,
    SERVO_4_STATE,

    // Effector info
    EFFECTOR_POSITION,
    EFFECTOR_SPEED,

    // Event flags
    FLAG_ARM,
    FLAG_DISARM,
    FLAG_ESTOP,
    FLAG_REHOME,

    FLAG_MODE_REQUEST,

    FLAG_EFFECTOR_NEAR_HOME,


    FLAG_SYNC_EPOCH,

    FLAG_PLANNER_VIOLATION,
    FLAG_EFFECTOR_VIOLATION,

    SYSTEM_NUM_FIELDS,  // Only marks end of enum range, shouldn't exceed 31 entries
} SYSTEM_EVENT_FLAG;

typedef enum
{
    MODE_UNKNOWN = 0,
    MODE_CHANGE,
    MODE_MANUAL,
    MODE_TRACK,
    MODE_DEMO,
    MODE_EVENT,
} ControlModes_t;

#endif    // SIGNALS_H
