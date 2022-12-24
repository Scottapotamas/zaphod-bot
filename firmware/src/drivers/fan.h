#ifndef FAN_H
#define FAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

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

PUBLIC void
fan_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_set_curve( FanCurve_t *curve, uint8_t num_points );

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_set( uint8_t speed_percentage );

/* -------------------------------------------------------------------------- */

PUBLIC uint8_t
fan_get_speed( void );

/* -------------------------------------------------------------------------- */

PUBLIC FanState_t
fan_get_state( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
fan_process( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* FAN_H */
