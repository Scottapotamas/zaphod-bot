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
	SYSTEM_STATUS_FAULT_DETECTED = STATE_USER_SIGNAL, // Signal 8 - First Custom Signal,
    SYSTEM_STATUS_REBOOTING,
    SYSTEM_STATUS_SETUP,

    /* User Activity Signals e.g. from terminal */
    USER_ACTIVITY_EVENT_SIGNAL,

    /* Button Signals */
    BUTTON_EVENT_SIGNAL,            /* Internal button activity event */
    BUTTON_PRESSED_SIGNAL,          /* Key Down Event */
    BUTTON_NORMAL_SIGNAL,           /* Normal Key Press Event */
    BUTTON_LONG_SIGNAL,             /* Long Key Press Event */
    BUTTON_RELEASED_SIGNAL,         /* Key Up Event */

    /* Servo Signals */
	MECHANISM_START,
	MECHANISM_STOP,
	MECHANISM_HOME,

    /* Motion Handler Signals */
	MOTION_PREPARE,			// Start n-axis homing process
	MOTION_REQUEST,         // Provide movement information for queue processing
	MOTION_EMERGENCY,       // Kill motors immediately
	MOTION_CLEAR_QUEUE,		// empty out pending movements

	MOTION_HOMED,			// all motors have homed without detected error
	MOTION_ERROR,			// error while homing
	MOTION_DISABLED,

    /* Last Available Signal - Don't Remove This */
    STATE_MAX_SIGNAL
};

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_SIGNALS_H */
