#ifndef USER_INTERFACE_MSGID_H
#define USER_INTERFACE_MSGID_H

#define MSGID_NICKNAME      "name"
#define MSGID_RESET_CAUSE   "reset_type"
#define MSGID_ASSERT_CAUSE  "assert_msg"
#define MSGID_FIRMWARE_INFO "fwb"
#define MSGID_KINEMATICS    "kinematics"
#define MSGID_ERROR         "err"

#define MSGID_SYSTEM     "sys"
#define MSGID_SUPERVISOR "super"
#define MSGID_MOTION     "moStat"
#define MSGID_FAN        "fan"
#define MSGID_SERVO      "servo"

#define MSGID_FAN_CURVE    "curve"
#define MSGID_MODE_REQUEST "req_mode"

#define MSGID_POSITION_TARGET    "tpos"
#define MSGID_POSITION_CURRENT   "cpos"
#define MSGID_POSITION_EXPANSION "exp_ang"

#define MSGID_LED                "rgb"
#define MSGID_LED_MANUAL_REQUEST "manual_led"

#define MSGID_QUEUE_INFO     "queue"
#define MSGID_QUEUE_SYNC     "sync"
#define MSGID_QUEUE_CLEAR    "clmv"
#define MSGID_QUEUE_ADD_MOVE "inmv"
#define MSGID_QUEUE_ADD_FADE "inlt"

#define MSGID_EMERGENCY_STOP "estop"
#define MSGID_ARM            "arm"
#define MSGID_DISARM         "disarm"
#define MSGID_HOME           "home"
#define MSGID_CAPTURE        "capture"

#define MSGID_LED_CALIBRATION "calLED"

// TODO: reimplement configuration messages
//        EUI_FLOAT( "rotZ", z_rotation ),
//        EUI_CUSTOM( "ledset", rgb_led_settings ),
//        EUI_CUSTOM( "pwr_cal", power_trims ),

#endif    // USER_INTERFACE_MSGID_H
