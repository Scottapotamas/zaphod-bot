#ifndef FAN_H
#define FAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

/**
 * @brief Possible states of the controlled fan
 */
typedef enum
{
    FAN_STATE_OFF,      /// Fan is off
    FAN_STATE_STALL,    /// Fan is currently stalled
    FAN_STATE_START,    /// Fan in startup blip phase
    FAN_STATE_ON,       /// Fan running normally
} FanState_t;

/**
 * @brief Represents a single control point in a fan speed control curve
 *
 * This structure defines a point in a fan speed control curve, used in an interpolation/lookup table for curve control.
 * It associates a temperature value (in Celsius) with a corresponding duty-cycle setpoint for the fan.
 * A curve can be defined as an array of these structures, passed by the user.
 */
typedef struct
{
    uint8_t temperature;    /// The temperature in Celsius at this point of the curve
    uint8_t percentage;     /// The fan's duty-cycle setpoint corresponding to the temperature
} FanCurve_t;

/* -------------------------------------------------------------------------- */

/**
 * @brief Initialize the fan.
 *
 * This function should be called before any other fan functions.
 */
PUBLIC void fan_init( void );

/* -------------------------------------------------------------------------- */

/**
 * @brief FreeRTOS compatible task
 *
 * Catch system events, run the fan control state-machine
 *
 * @param arg Unused
 */
PUBLIC void fan_task( void *arg );

/* -------------------------------------------------------------------------- */

/**
 * @brief Set a user fan curve.
 *
 * @param curve Pointer to an array of FanCurve_t 'points'
 * @param num_points The number of curve points in the pointed array
 */
PUBLIC void fan_set_curve( FanCurve_t *curve, uint8_t num_points );

/* -------------------------------------------------------------------------- */

/**
 * @brief Get the current state of the fan.
 *
 * @return The current state of the fan as an enum value.
 */
PUBLIC FanState_t fan_get_state( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* FAN_H */
