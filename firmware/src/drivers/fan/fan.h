#ifndef FAN_H
#define FAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"
#include "observer.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    FAN_STATE_OFF,
    FAN_STATE_STALL,
    FAN_STATE_START,
    FAN_STATE_ON,
} FanState_t;

typedef struct
{
    uint8_t temperature;
    uint8_t percentage;
} FanCurve_t;

/* -------------------------------------------------------------------------- */

PUBLIC void fan_init( void );

PUBLIC void fan_task( void *arg );

/* -------------------------------------------------------------------------- */

PUBLIC Observer * fan_get_observer( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_set_curve( FanCurve_t *curve, uint8_t num_points );

/* -------------------------------------------------------------------------- */

PUBLIC FanState_t
fan_get_state( void );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* FAN_H */
