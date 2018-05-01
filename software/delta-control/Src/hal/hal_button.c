/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "hal_button.h"
#include "hal_gpio.h"

/* -------------------------------------------------------------------------- */


/* ----- Defines ------------------------------------------------------------ */


/* ------------------------ Variables --------------------------------------- */


/* ----- Public Functions --------------------------------------------------- */

PUBLIC bool
hal_button_is_pressed( ButtonId_t id )
{
    bool pressed = false;
    switch( id )
    {
        case BUTTON_0:
            pressed = hal_gpio_read_pin(_BTN_0);
            break;
        case BUTTON_1:
            pressed = hal_gpio_read_pin(_BTN_1);
            break;
        default:
            break;
    }
    return pressed;
}

/* ----- End ---------------------------------------------------------------- */

