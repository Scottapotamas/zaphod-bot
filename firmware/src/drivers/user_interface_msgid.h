#ifndef USER_INTERFACE_MSGID_H
#define USER_INTERFACE_MSGID_H

#define MSGID_RESET_CAUSE   "rt"
#define MSGID_ASSERT_CAUSE  "am"
#define MSGID_FIRMWARE_INFO "fwb"
#define MSGID_ERROR         "err"

#define MSGID_KINEMATICS "kinematics"
#define MSGID_SYSTEM     "sx"
#define MSGID_SUPERVISOR "ss"
#define MSGID_MOTION     "sm"
#define MSGID_FAN        "sf"
#define MSGID_SERVO      "sc"

#define MSGID_FAN_CURVE    "curve"
#define MSGID_MODE_REQUEST "mr"

#define MSGID_POSITION_TARGET    "pt"
#define MSGID_POSITION_CURRENT   "pc"
#define MSGID_POSITION_EXPANSION "exp_ang"

#define MSGID_LED                "rgb"
#define MSGID_LED_MANUAL_REQUEST "manual_led"

#define MSGID_QUEUE_INFO     "qi"
#define MSGID_QUEUE_SYNC     "qs"
#define MSGID_QUEUE_CLEAR    "qc"
#define MSGID_QUEUE_ADD_MOVE "qm"
#define MSGID_QUEUE_ADD_FADE "qf"

#define MSGID_EMERGENCY_STOP "E"
#define MSGID_ARM            "A"
#define MSGID_DISARM         "D"
#define MSGID_HOME           "H"
#define MSGID_CAPTURE        "C"

#define MSGID_LED_CALIBRATION "calLED"

// TODO: reimplement configuration messages
//        EUI_FLOAT( "rotZ", z_rotation ),
//        EUI_CUSTOM( "ledset", rgb_led_settings ),
//        EUI_CUSTOM( "pwr_cal", power_trims ),

#endif    // USER_INTERFACE_MSGID_H