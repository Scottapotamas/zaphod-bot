#ifndef BUZZER_H
#define BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
buzzer_init( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
buzzer_sound( uint8_t count, uint16_t frequency, uint16_t duration_ms );

/* -------------------------------------------------------------------------- */

PUBLIC void
buzzer_mute( bool muted );

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* BUZZER_H */
