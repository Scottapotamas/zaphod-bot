#ifndef APP_SIGNALS_H
#define APP_SIGNALS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "state_event.h"

/* ----- Types -------------------------------------------------------------- */

enum AppSignals
{
    /* System Command Signals */
    SYSTEM_STATUS_FAULT_DETECTED = STATE_USER_SIGNAL,    // Signal 8 - First Custom Signal,

    /* User Activity Signals e.g. from terminal */
    USER_ACTIVITY_EVENT_SIGNAL,

    /* Button Signals */
    BUTTON_EVENT_SIGNAL,    /* Internal button activity event */
    BUTTON_PRESSED_SIGNAL,  /* Key Down Event */
    BUTTON_NORMAL_SIGNAL,   /* Normal Key Press Event */
    BUTTON_LONG_SIGNAL,     /* Long Key Press Event */
    BUTTON_RELEASED_SIGNAL, /* Key Up Event */

    /* High level mode change requests */
    MODE_TRACK,
    MODE_DEMO,
    MODE_EVENT,
    MODE_MANUAL,

    TRACKED_EXTERNAL_SERVO_REQUEST,
    TRACKED_TARGET_REQUEST,
    MOVEMENT_REQUEST,

    /* Servo Signals */
    MECHANISM_START,
    MECHANISM_STOP,
    MECHANISM_REHOME,

    MOTION_HOMED,    // all motors have homed without detected error
    MOTION_ERROR,    // error while homing
    MOTION_DISABLED,

    /* Motion Handler Signals */
    MOTION_PREPARE,      // Start n-axis homing process
    MOTION_EMERGENCY,    // Kill motors immediately

    MOTION_QUEUE_START,
    MOTION_QUEUE_ADD,      // Provide movement information for queue processing
    MOTION_QUEUE_CLEAR,    // empty out pending movements

    PATHING_STARTED,     // started executing a move
    PATHING_COMPLETE,    // finished moving along a provided profile path


    /* LED Handler Signals */
    LED_OFF,    // Turn off led immediately, clear the queue
    LED_QUEUE_START,
    LED_QUEUE_ADD,      // Provide led animation object to queue
    LED_QUEUE_CLEAR,    // empty pending animations

    ANIMATION_STARTED,  // started executing a fade
    ANIMATION_COMPLETE,    // finished drawing out the led animated colour ramp

    LED_ALLOW_MANUAL_CONTROL,
    LED_RESTRICT_MANUAL_CONTROL,

    LED_MANUAL_SET,

    QUEUE_SYNC_START,    // Provide a start signal to tasks which follow the id indexed queues. Should contain a SyncTimestampEvent

    CAMERA_CAPTURE,

    /* Last Available Signal - Don't Remove This */
    STATE_MAX_SIGNAL
};

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_SIGNALS_H */
