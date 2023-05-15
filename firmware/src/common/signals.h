#ifndef SIGNALS_H
#define SIGNALS_H

typedef enum _SYSTEM_EVENT_FLAG {

    // ADC backed events
    // Don't re-order
    SENSOR_SERVO_CURRENT = 0,
    SENSOR_VOLTAGE_INPUT,
    SENSOR_TEMPERATURE_PCB,
    SENSOR_TEMPERATURE_REGULATOR,
    SENSOR_TEMPERATURE_EXTERNAL,
    SENSOR_TEMPERATURE_MICRO,
    SENSOR_VOLTAGE_INTERNAL_REF,

    // Input capture backed events
    // Don't re-order
    SENSOR_SERVO_HLFB,
    SENSOR_FAN_SPEED,


    // Servo info
    SERVO_POWER,
    SERVO_POSITION,
    SERVO_SPEED,
    SERVO_STATE,

    // Effector info
    EFFECTOR_POSITION,
    EFFECTOR_SPEED,
    EFFECTOR_NEAR_HOME,

    // Event flags
    FLAG_ARM,
    FLAG_DISARM,
    FLAG_ESTOP,
    FLAG_REHOME,

    FLAG_MODE_REQUEST,
    FLAG_REQUEST_QUEUE_CLEAR,
    FLAG_REQUEST_SHUTTER_RELEASE,
    FLAG_SYNC_EPOCH,

    // Status update events
    FLAG_PLANNER_VIOLATION,
    FLAG_EFFECTOR_VIOLATION,

    OVERWATCH_STATE_UPDATE,
    OVERWATCH_MODE_UPDATE,

    QUEUE_UTILISATION_MOVEMENT,
    QUEUE_UTILISATION_LIGHTING,

    // Supervisor commands
    OVERWATCH_SERVO_ENABLE,
    OVERWATCH_SERVO_DISABLE,


    SYSTEM_NUM_FIELDS,  // Only marks end of enum range, shouldn't exceed 64 entries
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


enum BuzzerDefines
{
    BUZZER_TONE_LOW    = 500,
    BUZZER_TONE_NORMAL = 2000,
    BUZZER_TONE_HIGH   = 6000,

    BUZZER_ARMING_START_NUM      = 1,
    BUZZER_ARMING_START_DURATION = 100,
    BUZZER_ARMING_START_TONE     = 1500,

    BUZZER_ARMING_ERROR_NUM      = 3,
    BUZZER_ARMING_ERROR_DURATION = 200,
    BUZZER_ARMING_ERROR_TONE     = 800,

    BUZZER_ARMING_OK_NUM      = 2,
    BUZZER_ARMING_OK_DURATION = 80,
    BUZZER_ARMING_OK_TONE     = 3000,

    BUZZER_MODE_CHANGE_NUM      = 1,
    BUZZER_MODE_CHANGE_DURATION = 50,
    BUZZER_MODE_CHANGE_TONE     = 2000,

    BUZZER_MODE_CHANGED_NUM      = 1,
    BUZZER_MODE_CHANGED_DURATION = 50,
    BUZZER_MODE_CHANGED_TONE     = 4000,
};

#endif    // SIGNALS_H
