#ifndef APP_TIMES_H
#define APP_TIMES_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- Types -------------------------------------------------------------- */

#include "app_config.h" // For MS_TO_TICKS macro

/* -------------------------------------------------------------------------- */

enum AppTimeoutsMs
{
    TIME_BUTTON_POLL_MS                      =     200U, /* ms */
    TIME_BUTTON_DEFAULTS_ACTIVATE_MS         =    5000U, /* ms */

    TIME_BUTTON_NORMAL_PRESS                 =      50U, /* ms */
    TIME_BUTTON_LONG_PRESS                   =    1500U, /* ms */

};

/* -------------------------------------------------------------------------- */

enum SystemDefines
{
	BACKGROUND_RATE_BUTTON_MS	= 50U,	//  20Hz
	BACKGROUND_RATE_BUZZER_MS	= 10U,	// 100Hz
};

/* -------------------------------------------------------------------------- */

enum CommunicationDefines
{
	MODULE_BAUD		= 115200,
	INTERNAL_BAUD	= 115200,
	EXTERNAL_BAUD	= 115200,
};

/* -------------------------------------------------------------------------- */

enum FanDefines
{
	FAN_EVALUATE_TIME		= 500U,	//   2Hz
	FAN_STARTUP_TIME_MS		= 1000U,
	FAN_STALL_FAULT_RPM		= 20U,
	FAN_FREQUENCY_HZ		= 25000UL,
};

/* -------------------------------------------------------------------------- */

enum ServoDefines
{

#ifdef EXPANSION_SERVO
	SERVO_COUNT					 			= 4U,
#else
	SERVO_COUNT					 			= 3U,
#endif

	//Clearpath motion range allowed by mechanism
	SERVO_STEPS_PER_REV				= 6400U,
	SERVO_ANGLE_PER_REV				= 360U,
	SERVO_STEPS_PER_DEGREE			= ( SERVO_STEPS_PER_REV / SERVO_ANGLE_PER_REV ),
	SERVO_MIN_ANGLE					= 66U,	//this is the negative angle limit
	SERVO_ZERO_ANGLE				= 0U,	//bicep elbow is parallel to motor axis
	SERVO_MAX_ANGLE					= 60U,	//arm is fully extended
	SERVO_MIN_STEPS					= ( SERVO_MIN_ANGLE * SERVO_STEPS_PER_DEGREE ),
	SERVO_MAX_STEPS					= ( SERVO_MAX_ANGLE * SERVO_STEPS_PER_DEGREE ),

	SERVO_HOME_OFFSET				= 150U,

	//Homing parameters
	SERVO_INTERRUPTED_DISABLE_DELAY_MIN_MS	= 50U,
	SERVO_HOMING_MIN_MS 					= 100U,
	SERVO_HOMING_MAX_MS 					= 5000U,
	SERVO_HOMING_NULL_PERIODS_ALLOWED 		= 2U,
	SERVO_HOMING_SUPERVISOR_CHECK_MS		= 500U,
	SERVO_HOMING_SUPERVISOR_RETRIES			= (SERVO_HOMING_MAX_MS / SERVO_HOMING_SUPERVISOR_CHECK_MS) + 1,

	//Clearpath will filter pulses shorter than 1us
	//ULN2303 NPN driver has rise time of ~5ns, fall of ~10nsec
	SERVO_PULSE_DURATION_US 				= 4U,

	//Error evaluation parameters
	SERVO_IDLE_POWER_ALERT_W 				= 40U,
	SERVO_IDLE_SETTLE_MS					= 30U,
	SERVO_IDLE_POWER_TRIP_MS				= 400U,
	SERVO_OC_FAULT							= false,
	SERVO_OC_OK								= true,

	//Fault handling
	SERVO_FAULT_LINGER_MS 					= 8000U,

	//Clearpath input high = clockwise rotation. Alias against pin state
	SERVO_DIR_CCW							= true,
	SERVO_DIR_CW							= false,

	//Clearpath EN high = enable/GO
	SERVO_ENABLE							= true,
	SERVO_DISABLE							= false,
};

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TIMES_H */
