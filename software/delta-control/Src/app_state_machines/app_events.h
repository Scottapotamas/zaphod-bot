#ifndef APP_EVENTS_H
#define APP_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "app_config.h"
#include "state_task.h"
#include "state_event.h"
#include "button.h"
#include "motion_types.h"

/* -------------------------------------------------------------------------- */

/** Signal used in count down events */
typedef struct CountdownEvent__
{
    StateEvent  super;             /**< Encapsulated event reference */
    uint8_t     seconds;           /**< Countdown to 0 */
} CountdownEvent;

/* -------------------------------------------------------------------------- */

/** Internal report of button event */
typedef struct ButtonEvent__
{
    StateEvent        super;             /**< Encapsulated event reference */
    ButtonId_t        id;                /**< ID number of button event */
    ButtonPressType_t press_type;        /**< Type of button event */
} ButtonEvent;

/* -------------------------------------------------------------------------- */

/** Report detected button presses */
typedef struct ButtonPressedEvent__
{
    StateEvent    super;             /**< Encapsulated event reference */
    ButtonId_t    id;                /**< ID number of button event */
} ButtonPressedEvent;

/* -------------------------------------------------------------------------- */

/** Motion command */
typedef struct MotionPlannerEvent__
{
    StateEvent	super;     // Encapsulated event reference
    Movement_t	move;      // Movement details
} MotionPlannerEvent;

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_EVENTS_H */
