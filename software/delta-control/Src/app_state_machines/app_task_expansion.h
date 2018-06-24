#ifndef APP_TASK_EXPANSION_H
#define APP_TASK_EXPANSION_H

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
typedef struct AppTaskExpansion AppTaskExpansion;

struct AppTaskExpansion
{
    // ~~~ Core task structure ~~~
    StateTask		super;

    // ~~~ Task Timers ~~~
    EventTimer		timer1;
    EventTimer		timer2;

    // ~~~ Task Variables ~~~
    uint8_t	instance;	//port in use (internal, external, etc)
    uint8_t	mode;		//mode for the port
};

typedef enum {
	EXPANSION_INTERNAL_0 = 0,
	EXPANSION_INTERNAL_1,
	EXPANSION_INTERNAL_2,
	EXPANSION_INTERNAL_3,	//DAC/ADC capable
	EXPANSION_INTERNAL_4,	//DAC/ADC capable

	EXPANSION_EXTERNAL_0,
	EXPANSION_EXTERNAL_1,
	EXPANSION_EXTERNAL_2,
	EXPANSION_EXTERNAL_3,

	NUM_EXPANSION_PORTS,
} ExpansionInstance_t;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC StateTask *
appTaskExpansionCreate(	AppTaskExpansion *me,
                        StateEvent    	 *eventQueueData[],
						const uint8_t    eventQueueSize,
						const ExpansionInstance_t instance );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_TASK_EXPANSION_H */
