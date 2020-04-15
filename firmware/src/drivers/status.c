/* ----- Local Includes ----------------------------------------------------- */

#include "status.h"
#include "hal_gpio.h"

/* -------------------------------------------------------------------------- */
/* --- INIT                                                               --- */
/* -------------------------------------------------------------------------- */

PUBLIC void
status_init( void )
{
    /* assume pins are already init */
}

/* -------------------------------------------------------------------------- */
/* --- RED                                                                --- */
/* -------------------------------------------------------------------------- */

PUBLIC void
status_red( bool on )
{
    hal_gpio_write_pin( _STATUS_0, on );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
status_red_toggle( void )
{
    hal_gpio_toggle_pin( _STATUS_0 );
}

/* -------------------------------------------------------------------------- */
/* --- YELLOW                                                              --- */
/* -------------------------------------------------------------------------- */

PUBLIC void
status_yellow( bool on )
{
    hal_gpio_write_pin( _STATUS_1, on );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
status_yellow_toggle( void )
{
    hal_gpio_toggle_pin( _STATUS_1 );
}

/* -------------------------------------------------------------------------- */
/* --- GREEN                                                              --- */
/* -------------------------------------------------------------------------- */

PUBLIC void
status_green( bool on )
{
    hal_gpio_write_pin( _STATUS_2, on );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
status_green_toggle( void )
{
    hal_gpio_toggle_pin( _STATUS_2 );
}

/* -------------------------------------------------------------------------- */
/* --- EMERGENCY STOP                                                     --- */
/* -------------------------------------------------------------------------- */

bool external_state = false;

PUBLIC void
status_external( bool on )
{
    hal_gpio_write_pin( _EXT_OUTPUT_0, on );
    external_state = on;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
status_external_toggle( void )
{
    hal_gpio_toggle_pin( _EXT_OUTPUT_0 );
    external_state = !external_state;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
status_external_override( bool on )
{
    hal_gpio_write_pin( _EXT_OUTPUT_0, on );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
status_external_resume( void )
{
    hal_gpio_write_pin( _EXT_OUTPUT_0, external_state );
}

/* ----- End ---------------------------------------------------------------- */
