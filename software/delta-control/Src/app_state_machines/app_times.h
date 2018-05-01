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
    TIME_APPLICATION_BATTERY_CHECK           = (14*60*1000), /* 14 minutes */
    TIME_APPLICATION_BATTERY_FAST_SAMPLE     =    100U, /* ms */
    TIME_APPLICATION_BATTERY_SLOW_SAMPLE     =   1000U, /* ms */
    TIME_APPLICATION_STEP                    =    200U, /* ms */
    TIME_APPLICATION_TICK                    =     20U, /* ms */
    TIME_APPLICATION_WAITING_TICK            =    300U, /* ms */
    TIME_APPLICATION_COUNTDOWN               =   1000U, /* ms */
    TIME_APPLICATION_FIRED_WAIT              =   1000U, /* ms */
    TIME_APPLICATION_CAPTURE_WAIT            =   1000U, /* ms */
    TIME_APPLICATION_CAPTURE_TIMEOUT         =  10000U, /* ms */
    TIME_APPLICATION_SAVE_PHOTO_ERROR        =  30000U, /* ms */
    TIME_APPLICATION_SAVE_LOG_ERROR          =  30000U, /* ms */
    TIME_APPLICATION_SPLASH_SCREEN           =   2000U, /* ms */
    TIME_APPLICATION_SPLASH_SHORT            =   2000U, /* ms */
    TIME_APPLICATION_SPLASH_LONG             =   5000U, /* ms */
    TIME_APPLICATION_SYSTEM_STATUS_POLL      = 600000U, /* ms */
    TIME_APPLICATION_BATTERY_POLL            =    100U, /* ms */
    TIME_APPLICATION_USB_MOUNT_POLL          =   3000U, /* ms */
    TIME_APPLICATION_ERROR_CONTINUE          =  10000U, /* ms */
    TIME_APPLICATION_USB_ERROR_CONTINUE      =  30000U, /* ms */
    TIME_APPLICATION_USB_POWERON             =    200U, /* ms */
    TIME_APPLICATION_USB_POWEROFF            =   1000U, /* ms */
    TIME_APPLICATION_USB_DETECT              =  10000U, /* ms */
    TIME_APPLICATION_USB_IDLE                =  10000U, /* ms */
    TIME_APPLICATION_MENU_PLAY               =   5000U, /* ms */
    TIME_APPLICATION_FILE_SYSTEM_CHECK       =   1000U, /* ms */
    TIME_APPLICATION_USB_RETRY               =   1000U, /* ms */
    TIME_APPLICATION_BATTERY_CRITICAL        =   5000U, /* ms */
    TIME_APPLICATION_INACTIVITY              = (10*60*1000), /* ms */

    TIME_APPLICATION_ERROR_STATE             =  60000U, /* ms */
    TIME_APPLICATION_CAMERA_CHECK            =  10000U, /* ms */
    TIME_APPLICATION_DELIVERY_CHECK          =  10000U, /* ms */
    TIME_APPLICATION_SENSOR_CHECK            =  10000U, /* ms */

    TIME_MENU_TIMEOUT                        = (2*60*1000), /* ms */
    TIME_MENU_REFRESH                        =    1000U, /* ms */
    TIME_MENU_REFRESH_FAST                   =     300U, /* ms */
    TIME_MENU_REFRESH_SLOW                   =    5000U, /* ms */
    TIME_MENU_SENSOR_REFRESH                 =     500U, /* ms */
    TIME_MINUTE_TICK                         =   60000U, /* ms */
    TIME_BATTERY_CHECK_REFRESH               =     300U, /* ms */
    TIME_BATTERY_CHECK_FLASH                 =     300U, /* ms */
    TIME_BACKLIGHT_TIMEOUT                   =   60000U, /* ms */
    TIME_POWER_OFF_TIMEOUT                   =   10000U, /* ms */

    TIME_LEVELLING_LASER_TIMEOUT             =   60000U, /* ms */

    TIME_BUTTON_POLL_MS                      =     200U, /* ms */
    TIME_BUTTON_DEFAULTS_ACTIVATE_MS         =    5000U, /* ms */

    TIME_BUTTON_NORMAL_PRESS                 =      50U, /* ms */
    TIME_BUTTON_LONG_PRESS                   =    1500U, /* ms */

    TIME_MAGAZINE_LATCH_OPEN                 =     300U, /* ms */
    TIME_MAGAZINE_LATCH_CLOSE                =    1000U, /* ms */
    TIME_MAGAZINE_LATCH_BACKOFF              =      50U, /* ms */
    TIME_MAGAZINE_LATCH_RELEASE              =     350U, /* ms */
    TIME_MAGAZINE_LATCH_PAUSE                =    2000U, /* ms */
    TIME_MAGAZINE_MOTOR_HOME_LATCH           =    5000U, /* ms */
    TIME_MAGAZINE_MOTOR_HOME_RETRY           =    5000U, /* ms */
    TIME_MAGAZINE_MOTOR_HOME_SEEK_TIMEOUT    =    3000U, /* ms */
    TIME_MAGAZINE_MOTOR_HOME_SEEK_RUN_TIME   =    2000U, /* ms */
    TIME_MAGAZINE_MOTOR_HOME_TIMEOUT         =   30000U, /* ms */
    TIME_MAGAZINE_MOTOR_CHECK_FORWARD_TIMEOUT=    1000U, /* ms */
    TIME_MAGAZINE_MOTOR_CHECK_TIMEOUT        =    3000U, /* ms */
    TIME_MAGAZINE_MOTOR_SEEK_REVERSE_TIME    =     500U, /* ms */
    TIME_MAGAZINE_MOTOR_FAULT_TIME           =    3000U, /* ms */
    TIME_MAGAZINE_MOTOR_FORWARD_TIMEOUT      =    7000U, /* ms */
    TIME_MAGAZINE_MOTOR_FORWARD_RUN_TIME     =    1000U, /* ms */
    TIME_MAGAZINE_MOTOR_FORWARD_SLOT_TIME    =     750U, /* ms */
    TIME_MAGAZINE_MOTOR_FORWARD_MINIMUM_TIME =    1000U, /* ms */
    TIME_MAGAZINE_MOTOR_FORWARD_RETRY_TIME   =   10000U, /* ms */
    TIME_MAGAZINE_MOTOR_REVERSE_TIMEOUT      =  120000U, /* ms */
    TIME_MAGAZINE_MOTOR_REVERSE_RETRY_TIME   =   10000U, /* ms */
    TIME_MAGAZINE_MOTOR_REVERSE_SLOT_TIMEOUT =   10000U, /* ms */
    TIME_MAGAZINE_MOTOR_REVERSE_SLOT_TIME    =    1100U, /* ms */
    TIME_MAGAZINE_MOTOR_REVERSE_MINIMUM_TIME =    1000U, /* ms */
	TIME_MAGAZINE_MOTOR_PAUSE_TIME           =     500U, /* ms */
 	TIME_MAGAZINE_MOTOR_ERROR_TIMEOUT        =   10000U, /* ms */

    TIME_PISTON_POWERUP                      =     500U, /* ms */
    TIME_PISTON_RETRIEVE_TIMEOUT             =   60000U, /* ms */
    TIME_PISTON_RETRIEVE_RUN_TIME            =   12000U, /* ms */
    TIME_PISTON_RETRIEVE_RETRY               =   12000U, /* ms */
    TIME_PISTON_TURN_AROUND_TIMEOUT          =     500U, /* ms */
    TIME_PISTON_CHARGE_TIMEOUT               =   60000U, /* ms */
    TIME_PISTON_CHARGE_A_BIT_STOP_TIME       =    4000U, /* ms */
    TIME_PISTON_CHARGE_A_BIT_RUN_TIME        =    5000U, /* ms */
    TIME_PISTON_CHARGE_RUN_TIME              =   15000U, /* ms */
    TIME_PISTON_TRIGGER_FIRE                 =    1000U, /* ms */
    TIME_PISTON_TRIGGER_PAUSE                =   10000U, /* ms */

    TIME_CAMERA_POWER_UP_CHECK               =     100U, /* ms */
    TIME_CAMERA_POWER_UP_RETRY               =    1000U, /* ms */
    TIME_CAMERA_POWER_SWITCH_CHECK           =      10U, /* ms */
    TIME_CAMERA_POWER_UP_DELAY               =     500U, /* ms */
    TIME_CAMERA_PAUSE_BEFORE_SAVE            =    1000U, /* ms */
    TIME_CAMERA_RESET_TIMEOUT                =   10000U, /* ms */
    TIME_CAMERA_RESPONSE_TIMEOUT             =    5000U, /* ms */
    TIME_CAMERA_FLASH_OFF_DELAY              =      50U, /* ms */
    TIME_CAMERA_HEARTBEAT                    = (30*60*1000), /* ms */
    TIME_CAMERA_RESTART_TIMEOUT              =   30000U, /* ms */

    TIME_WILDSPY_ACTIVITY_TIMEOUT            =   90000U, /* ms */
    TIME_WILDSPY_RESTART_TIMEOUT             =   10000U, /* ms */

    TIME_SENSOR_TEST_WAIT                    =   10000U, /* ms */
    TIME_SENSOR_MAX_RANGE_WAIT               =    3000U, /* ms */
    TIME_SENSOR_MAX_RANGE_POLL               =   15000U, /* ms */
    TIME_SENSOR_POLL                         =      10U, /* ms */
    TIME_SENSOR_TEST_POLL                    =     250U, /* ms */
    TIME_SENSOR_POWER_ON_WAIT                =    1000U, /* ms */
    TIME_SENSOR_STANDBY_POLL                 =    1000U, /* ms */
    TIME_SENSOR_ACTIVE_TIMEOUT               =   30000U, /* ms */
    TIME_SENSOR_ACTIVE_POLL                  =   10000U, /* ms */
    TIME_SENSOR_RESTART_TIME                 =    2000U, /* ms */
    TIME_SENSOR_AVERAGE_SHORT                =    1000U, /* ms */
    TIME_SENSOR_AVERAGE_LONG                 =   60000U, /* ms */
    TIME_SENSOR_STANDBY_REFRESH              =     100U, /* ms */
    TIME_SENSOR_COOLDOWN_REFRESH             =     250U, /* ms */

    TIME_LOG_WRITE_DELAY                     =  600000U, /* 10 minutes */
    TIME_LOG_WRITE_DELAY_SHORT               =   20000U, /* 20 seconds */

    TIME_AMPLIFIER_POWER_UP                  =    100U, /* ms */
};

/* -------------------------------------------------------------------------- */

enum AppCounters
{
    COUNT_MAX_USB_MOUNT_RETRIES              =     3U,
    COUNT_CAMERA_COMMUNICATION_RETRIES       =    10U,
    COUNT_CAMERA_POWER_RETRIES               =     3U,
    COUNT_CAMERA_POWER_TRY_COUNT             =    30U,
    COUNT_CAMERA_POWER_GOOD_COUNT            =     5U,
    COUNT_POWER_OFF                          =    10U,
    COUNT_HIBERNATE                          =    10U,
    COUNT_ACTIVATING                         =    30U,
    COUNT_ACTIVATING_FACTORY_MODE            =     5U,
    COUNT_BATTERY_CHECKS                     =    20U,  /* Number of times averaging battery */
};

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TIMES_H */
