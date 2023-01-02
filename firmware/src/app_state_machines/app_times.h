#ifndef APP_TIMES_H
#define APP_TIMES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "global.h"

/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */

enum AppTimeoutsMs
{
    TIME_BUTTON_POLL_MS              = 100U,  /* ms */
    TIME_BUTTON_DEFAULTS_ACTIVATE_MS = 5000U, /* ms */

    TIME_BUTTON_NORMAL_PRESS = 50U,   /* ms */
    TIME_BUTTON_LONG_PRESS   = 1500U, /* ms */
};

/* -------------------------------------------------------------------------- */

enum SystemDefines
{
    ADC_SAMPLE_RATE_MS         = 10U,    // 50Hz
    BACKGROUND_RATE_HARD_IC_MS = 5U,     //  100Hz
    SPEED_ESTIMATOR_SPAN       = 50U,    // samples in rolling buffer
    BACKGROUND_RATE_BUTTON_MS  = 20U,     //  50Hz
    BACKGROUND_RATE_BUZZER_MS  = 10U,     // 100Hz
    BACKGROUND_ADC_AVG_POLL_MS = 100U,    //  10Hz

    MOVEMENT_QUEUE_DEPTH_MAX = 150U,    // movement events in the queue
    LED_QUEUE_DEPTH_MAX      = 250U,    // LED animations in the queue

};

/* -------------------------------------------------------------------------- */

enum CommunicationDefines
{
    MODULE_BAUD   = 500000,
    INTERNAL_BAUD = 115200,
    EXTERNAL_BAUD = 115200,
};

/* -------------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------- */

enum FanDefines
{
    FAN_EVALUATE_TIME      = 100U,
    FAN_STARTUP_TIME_MS    = 1000U,
    FAN_STALL_WAIT_TIME_MS = 3000U,
    FAN_STALL_FAULT_RPM    = 140U,
    FAN_STALL_MAX_RPM      = 1800U,
    FAN_FREQUENCY_HZ       = 25000UL,
    NUM_FAN_CURVE_POINTS   = 5U,
};

/* -------------------------------------------------------------------------- */

enum LedDefines
{
    LED_FREQUENCY_HZ = 8000U,   // TODO check timer output is accurate/prescale correct
    LED_SLEEP_TIMER  = 2500U,
};

/* -------------------------------------------------------------------------- */

enum KinematicsDefines
{
    KINEMATICS_Z_MAX_MM = 200U,
    KINEMATICS_Z_MIN_MM = 0U,
    KINEMATICS_RADIUS_MAX_MM = 225U,

    KINEMATICS_Z_OFFSET_MM = 190U,
};

/* -------------------------------------------------------------------------- */

enum PathingDefines
{
    EFFECTOR_SPEED_LIMIT    = 650U,    // mm/second
    SPEED_SAMPLE_RESOLUTION = 20U,     // number of samples to sum across spline
};

/* -------------------------------------------------------------------------- */

enum ServoDefines
{
    SERVO_COUNT = 4U,

    // Clearpath motion range allowed by mechanism
    SERVO_STEPS_PER_REV    = 6400U,
    SERVO_ANGLE_PER_REV    = 360U,
    SERVO_STEPS_PER_DEGREE = ( SERVO_STEPS_PER_REV / SERVO_ANGLE_PER_REV ),
    SERVO_MIN_ANGLE        = 45U,    // this is the negative angle limit
    SERVO_ZERO_ANGLE       = 0U,     // bicep elbow is parallel to motor axis
    SERVO_MAX_ANGLE        = 65U,    // arm is fully extended
    SERVO_MIN_STEPS        = ( SERVO_MIN_ANGLE * SERVO_STEPS_PER_DEGREE ),
    SERVO_MAX_STEPS        = ( SERVO_MAX_ANGLE * SERVO_STEPS_PER_DEGREE ),

    SERVO_HOME_OFFSET = 25U,

    // Homing parameters
    SERVO_HOMING_CALIBRATION_SAMPLES = 10U,
    SERVO_HOMING_CALIBRATION_MS      = ( SERVO_HOMING_CALIBRATION_SAMPLES * 22U ),    // 45hz -> 22ms per sample,
    SERVO_HOMING_CALIBRATION_TORQUE  = 10U,                                           // maximum torque we expect to see during torque calibration

    SERVO_HOMING_ENDSTOP_TRANSIT_TORQUE  = 20U,     // shutdown if servo exceeds this limit during the move towards the endstop
    SERVO_HOMING_ENDSTOP_RAMP_MIN        = 15U,     // lower limit % of torque expected to trigger homing endstop behaviour
    SERVO_HOMING_ENDSTOP_RAMP_MAX        = 25U,     // upper limit % of torque expected to home
    SERVO_HOMING_FOLDBACK_CHECK_START_MS = 200U,    // ms after peak to start checking foldback level
    SERVO_HOMING_FOLDBACK_CHECK_END_MS   = 500U,    // ms after peak to check foldback is still ok
    SERVO_HOMING_FOLDBACK_TORQUE_MIN     = 4U,      // % torque min expected during foldback
    SERVO_HOMING_FOLDBACK_TORQUE_MAX     = 6U,      // % torque max expected during foldback
    SERVO_HOMING_SIMILARITY_PERCENT      = 1U,      // % torque error allowed during stabilisation period
    SERVO_HOMING_SIMILARITY_MS           = 200U,    // time the torque needs to be stable before considering homing move complete
    SERVO_HOMING_COMPLETE_MAX_MS         = 500U,

    SERVO_HOMING_MAX_MS = 9000U,

    SERVO_HOMING_NULL_PERIODS_ALLOWED = 2U,
    SERVO_HOMING_SUPERVISOR_CHECK_MS  = 500U,
    SERVO_HOMING_SUPERVISOR_RETRIES   = ( SERVO_HOMING_MAX_MS / SERVO_HOMING_SUPERVISOR_CHECK_MS ) + 1,

    SERVO_RECOVERY_DWELL_MS = 50U,
    SERVO_RECOVERY_RETRIES  = 7U,

    // Clearpath will filter pulses shorter than 1us
    // ULN2303 NPN driver has rise time of ~5ns, fall of ~10nsec
    SERVO_PULSE_DURATION_US = 10U,

    // Error evaluation parameters
    SERVO_IDLE_POWER_ALERT_W = 40U,
    SERVO_IDLE_TORQUE_ALERT  = 30U,
    SERVO_IDLE_SETTLE_MS     = 50U,
    SERVO_IDLE_LOAD_TRIP_MS  = 900U,
    SERVO_OC_FAULT           = false,
    SERVO_OC_OK              = true,

    // Fault handling
    SERVO_FAULT_LINGER_MS = 500U,
    SERVO_MISSING_HLFB_MS = 25U,    // ms since last HLFB input captures before declaring the servo missing

    // Clearpath input high = clockwise rotation. Alias against pin state
    SERVO_DIR_CCW = true,
    SERVO_DIR_CW  = false,

    // Clearpath EN high = enable/GO
    SERVO_ENABLE  = true,
    SERVO_DISABLE = false,
};

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TIMES_H */
