#ifndef APP_EVENTS_H
#define APP_EVENTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "app_config.h"
#include "button.h"
#include "global.h"
#include "led_types.h"
#include "motion_types.h"
#include "state_event.h"
#include "state_task.h"
#include "timer_ms.h"

/* -------------------------------------------------------------------------- */

/** Signal used in count down events */
typedef struct CountdownEvent__
{
    StateEvent super;   /**< Encapsulated event reference */
    uint8_t    seconds; /**< Countdown to 0 */
} CountdownEvent;

/* -------------------------------------------------------------------------- */

/** Internal report of button event */
typedef struct ButtonEvent__
{
    StateEvent        super;      /**< Encapsulated event reference */
    ButtonId_t        id;         /**< ID number of button event */
    ButtonPressType_t press_type; /**< Type of button event */
} ButtonEvent;

/** Report detected button presses */
typedef struct ButtonPressedEvent__
{
    StateEvent super; /**< Encapsulated event reference */
    ButtonId_t id;    /**< ID number of button event */
} ButtonPressedEvent;

/* -------------------------------------------------------------------------- */

/** Motion command */
typedef struct MotionPlannerEvent__
{
    StateEvent super;    // Encapsulated event reference
    Movement_t move;     // Movement details
} MotionPlannerEvent;

/* -------------------------------------------------------------------------- */

/** Lighting command */
typedef struct LedPlannerEvent__
{
    StateEvent super;        // Encapsulated event reference
    Fade_t     animation;    // LED animation details
} LightingPlannerEvent;

typedef struct LedManualEvent__
{
    StateEvent  super;     // Encapsulated event reference
    RGBColour_t colour;    // LED colour target
    bool        enabled;
} LightingManualEvent;

/* -------------------------------------------------------------------------- */

/** Tracked position request command */
typedef struct TrackedPositionRequestEvent__
{
    StateEvent       super;     // Encapsulated event reference
    CartesianPoint_t target;    // Position in 3D cartesian space, in microns
} TrackedPositionRequestEvent;

/* -------------------------------------------------------------------------- */

/** Tracked position request command */
typedef struct ExpansionServoRequestEvent__
{
    StateEvent super;     // Encapsulated event reference
    float      target;    // Target shaft angle in degrees
} ExpansionServoRequestEvent;

/* -------------------------------------------------------------------------- */

/** Motion and Lighting Barrier Message Data **/
typedef struct SyncTimestampEvent__
{
    StateEvent super;    // Encapsulated event reference
    timer_ms_t epoch;    // running time reference used for global 'timestamp id' sync
} SyncTimestampEvent;

/* -------------------------------------------------------------------------- */

typedef struct CameraShutterEvent__
{
    StateEvent super;    // Encapsulated event reference
    uint32_t   exposure_time;
} CameraShutterEvent;

/* -------------------------------------------------------------------------- */

typedef struct DemoModeConfigurationEvent__
{
    StateEvent super;    // Encapsulated event reference
    uint8_t    program_index;
} DemoModeConfigurationEvent;

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* APP_EVENTS_H */
