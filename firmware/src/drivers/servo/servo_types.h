#ifndef SERVO_TYPES_H
#define SERVO_TYPES_H

/* -------------------------------------------------------------------------- */

#define SERVO_STEPS_PER_REV (6400U)
#define SERVO_ANGLE_PER_REV (360U)
#define SERVO_MIN_ANGLE (45U)  // this is the negative angle limit
#define SERVO_MAX_ANGLE (65U)  // arm is fully extended
#define SERVO_IDLE_POWER_ALERT_W (30U)
#define SERVO_IDLE_TORQUE_ALERT (20U)
#define SERVO_IDLE_SETTLE_MS (50U)
#define SERVO_HOME_OFFSET (25U)

// ms since last HLFB input captures before declaring the servo missing
// As HLFB is 45Hz = 22ms, timeout is set to allow two missed cycles
#define SERVO_MISSING_HLFB_MS (50U)

// Clearpath input high = clockwise rotation. Alias against pin state
#define SERVO_DIR_CCW (true)
#define SERVO_DIR_CW (false)

#define SERVO_MAX_STEPS_PER_TICK 8


#define SERVO_HOMING_ENDSTOP_TRANSIT_TORQUE     (20U)     // shutdown if servo exceeds this limit during the move towards the endstop
#define SERVO_HOMING_ENDSTOP_RAMP_MIN           (15U)     // lower limit % of torque expected to trigger homing endstop behaviour
#define SERVO_HOMING_ENDSTOP_RAMP_MAX           (25U)     // upper limit % of torque expected to home
#define SERVO_HOMING_FOLDBACK_CHECK_START_MS    (200U)    // ms after peak to start checking foldback level
#define SERVO_HOMING_FOLDBACK_CHECK_END_MS      (500U)    // ms after peak to check foldback is still ok
#define SERVO_HOMING_FOLDBACK_TORQUE_MIN        (4U)      // % torque min expected during foldback
#define SERVO_HOMING_FOLDBACK_TORQUE_MAX        (6U)      // % torque max expected during foldback
#define SERVO_HOMING_SIMILARITY_PERCENT         (2U)      // % torque error allowed during stabilisation period
#define SERVO_HOMING_SIMILARITY_MS              (200U)    // time the torque needs to be stable before considering homing move complete
#define SERVO_HOMING_COMPLETE_MAX_MS            (500U)
#define SERVO_HOMING_MAX_MS                     (9000U)

#define SERVO_HOMING_CALIBRATION_SAMPLES  (20U)
#define SERVO_HOMING_CALIBRATION_MS       (SERVO_HOMING_CALIBRATION_SAMPLES * 22U)  // 45hz -> 22ms per sample
#define SERVO_HOMING_CALIBRATION_TORQUE   (10U)                                     // maximum torque we expect to see during torque calibration

#define SERVO_RECOVERY_DWELL_MS (50U)
#define SERVO_RECOVERY_RETRIES  (7U)


// Fault handling
#define SERVO_FAULT_LINGER_MS (500U)

// ms since last HLFB input captures before declaring the servo missing
// As HLFB is 45Hz = 22ms, timeout is set to allow two missed cycles
#define SERVO_MISSING_HLFB_MS (50U)

/* -------------------------------------------------------------------------- */

typedef enum
{
    _CLEARPATH_1 = 0,
    _CLEARPATH_2,
    _CLEARPATH_3,
    _CLEARPATH_4,
    _NUMBER_CLEARPATH_SERVOS
} ClearpathServoInstance_t;

/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */

#endif    // SERVO_TYPES_H