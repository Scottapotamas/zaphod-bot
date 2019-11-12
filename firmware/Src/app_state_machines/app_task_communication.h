#ifndef APP_TASK_COMMUNICATION_H
#define APP_TASK_COMMUNICATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */
#include "global.h"
#include "state_task.h"
#include "event_timer.h"

/* ----- State Task Control Block ------------------------------------------- */

/** Application Task Control Block File System */
typedef struct AppTaskCommunication AppTaskCommunication;

struct AppTaskCommunication
{
    // ~~~ Core task structure ~~~
    StateTask		super;

    // ~~~ Task Timers ~~~
    EventTimer		timer1;
    EventTimer		timer2;

    // ~~~ Task Variables ~~~
     uint8_t	instance;	//port in use (internal, external, etc)
};

typedef enum {
	INTERFACE_UART_MODULE,

	//Internal headers
	INTERFACE_UART_INTERNAL,
	INTERFACE_I2C_INTERNAL,

	//LEMO Connector
	INTERFACE_UART_EXTERNAL,
	INTERFACE_CAN_EXTERNAL,
	INTERFACE_USB_EXTERNAL,
	INTERFACE_COUNT
} CommunicationInstance_t;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskCommunicationCreate(	 AppTaskCommunication *me,
                         	 StateEvent        *eventQueueData[],
							 const uint8_t     eventQueueSize,
							 const CommunicationInstance_t instance );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_COMMUNICATION_H */
