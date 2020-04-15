#ifndef HAL_RESET_H
#define HAL_RESET_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* -------------------------------------------------------------------------- */

typedef enum
{
    RESET_CAUSE_UNKNOWN = 0,
    RESET_CAUSE_BOR,
    RESET_CAUSE_PIN,
    RESET_CAUSE_POR,
    RESET_CAUSE_SW,
    RESET_CAUSE_IWDG,
    RESET_CAUSE_WWDG,
    RESET_CAUSE_LPWR,
} HalPowerResetCause_t;

/* -------------------------------------------------------------------------- */

PUBLIC HalPowerResetCause_t
hal_reset_cause( void );

/* -------------------------------------------------------------------------- */

PUBLIC const char *
hal_reset_cause_description( HalPowerResetCause_t cause );

/* -------------------------------------------------------------------------- */

PUBLIC void
hal_reset_software( void );

/* ----- End ---------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* HAL_RESET_H */
