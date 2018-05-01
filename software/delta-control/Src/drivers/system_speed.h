#ifndef SYSTEM_SPEED_H
#define SYSTEM_SPEED_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include <stdbool.h>

/* ----- Public Functions --------------------------------------------------- */

#include "global.h"

typedef enum
{
    SYSTEM_SPEED_LOW = 0,
    SYSTEM_SPEED_FULL
} SystemSpeed_t;

/* -------------------------------------------------------------------------- */

PUBLIC void
system_speed_init( void );

/* -------------------------------------------------------------------------- */

/** Set the system state in terms of clock speed */

PUBLIC void
system_speed_set( uint8_t id, SystemSpeed_t speed );

/* -------------------------------------------------------------------------- */

/** Get the current system state in terms of clock speed */

PUBLIC SystemSpeed_t
system_speed_get( void );

/* -------------------------------------------------------------------------- */

/** Process the clock switch state machine */

PUBLIC void
system_speed_process( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_SPEED_H */
