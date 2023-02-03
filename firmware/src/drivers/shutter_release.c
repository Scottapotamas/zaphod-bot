/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "app_times.h"
#include "hal_gpio.h"
#include "shutter_release.h"
#include "simple_state_machine.h"
#include "timer_ms.h"

/* ----- Private Types ------------------------------------------------------ */

/* ----- Private Variables -------------------------------------------------- */

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
shutter_init( void )
{

}

/* -------------------------------------------------------------------------- */

PUBLIC void
shutter_capture( bool on )
{
    hal_gpio_write_pin( _EXT_OUTPUT_1, on );
}

/* -------------------------------------------------------------------------- */

/* ----- End ---------------------------------------------------------------- */
