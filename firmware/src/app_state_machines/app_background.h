#ifndef APP_BACKGROUND_H
#define APP_BACKGROUND_H

#ifdef __cplusplus
extern "C" {
#endif

/* ----- System Includes ---------------------------------------------------- */

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"

/* ----- Defines ------------------------------------------------------------ */

/* ----- Public Functions --------------------------------------------------- */

/** Startup initialisation for background processes in the main loop */

PUBLIC void
app_background_init( void );

/* -------------------------------------------------------------------------- */

/** Background processes that are handled in the main loop */

PUBLIC void
app_background( void );

/* ----- End ------------------------------~--------------------------------- */

#ifdef __cplusplus
}
#endif
#endif /* APP_BACKGROUND_H */
