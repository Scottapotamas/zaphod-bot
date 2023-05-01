#ifndef SENSORS_H
#define SENSORS_H

/* -------------------------------------------------------------------------- */

typedef enum _SENSOR_INDEX {
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

    SENSOR_SERVO_1_HLFB,
    SENSOR_SERVO_2_HLFB,
    SENSOR_SERVO_3_HLFB,
    SENSOR_SERVO_4_HLFB,
    SENSOR_FAN_SPEED,

    SENSOR_NUM_FIELDS,  // Only marks end of enum range, shouldn't exceed 31 entries
} SENSOR_INDEX;

/* -------------------------------------------------------------------------- */

void sensors_init( void );

/* -------------------------------------------------------------------------- */

void sensors_task( void *arg );

/* -------------------------------------------------------------------------- */

#endif    // SENSORS_H