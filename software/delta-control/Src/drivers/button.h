#ifndef BUTTON_H
#define BUTTON_H

/* ----- System Includes ---------------------------------------------------- */

#include <stdint.h>
#include <stdbool.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "global.h"
#include "hal_button.h"

/* ----- Defines ------------------------------------------------------------ */

#define BTN_LEFT        _BV(0)
#define BTN_MIDLE       _BV(1)
#define BTN_RIGHT       _BV(2)
#define BTN_POWER       _BV(3)

/* -------------------------------------------------------------------------- */

typedef enum
{
    BUTTON_PATTERN_EMERGENCY_SHUTDOWN = ( BTN_LEFT | BTN_MIDLE | BTN_RIGHT ),
    BUTTON_PATTERN_HIBERNATE          = ( BTN_LEFT | BTN_MIDLE | 0         ),
    BUTTON_PATTERN_FIRMWARE_UPDATE    = ( BTN_LEFT | 0         | BTN_RIGHT ),
} ButtonPattern_t;

/* ----- Types ------------------------------------------------------------- */

typedef enum
{
    BUTTON_PRESS_TYPE_DOWN = 0x00, /*!< Button pressed */
    BUTTON_PRESS_TYPE_NORMAL,      /*!< Normal press type, released */
    BUTTON_PRESS_TYPE_LONG,        /*!< Long press type */
    BUTTON_PRESS_TYPE_UP           /*!< Button released */
} ButtonPressType_t;

typedef void (*ButtonHandler_t)( ButtonId_t id, ButtonPressType_t press_type );

/* ----- Public Functions --------------------------------------------------- */

/* Configure the given button and set the call back handler
 * Returns true when the button is already pressed
 */

PUBLIC bool
button_init( ButtonId_t id, ButtonHandler_t handler );

/* -------------------------------------------------------------------------- */

PUBLIC bool
button_pattern_match( ButtonPattern_t pattern );

/* -------------------------------------------------------------------------- */

/* Background process checking button presses */

PUBLIC void
button_process( void );

/* -------------------------------------------------------------------------- */

#endif /* BUTTON_H */
