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
    SYSTEM_CMD_POWER_OFF = STATE_USER_SIGNAL, // Signal 8 - First Custom Signal,
    SYSTEM_STATUS_POWER_ON,

    SYSTEM_STATUS_FAULT_DETECTED,
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
	MECHANISM_HOMED,		// all motors have homed without detected error
	MECHANISM_ERROR,

    /* Motion Handler Signals */
	MOTION_PREPARE,
	MOTION_STOP,           /* Stop current movement actions */
	MOTION_REQUEST,        /* Provide movement information for queue processing */
	MOTION_HEAD,           /* Start of a movement */
	MOTION_TAIL,           /* End of a movement */
	MOTION_EMERGENCY,      /* Kill motors immediately */

#warning "This dirty hack should really really be removed at some point"
	EUI_PING,

    /* Last Available Signal - Don't Remove This */
    STATE_MAX_SIGNAL
};

/* ----------------------- End --------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_SIGNALS_H */
