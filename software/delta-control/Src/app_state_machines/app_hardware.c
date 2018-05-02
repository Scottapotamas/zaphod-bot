/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "hal_systick.h"
#include "hal_adc.h"
#include "status.h"
#include "hal_gpio.h"
#include "hal_watchdog.h"
#include "hal_system_speed.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
app_hardware_init( void )
{
    /* Configure internal GPIO pins */
    hal_gpio_configure_mcu_defaults();

    /* Start the watchdog for around 20s timeouts */
    hal_watchdog_init( 20000 );

    /* Init the CPU manager DWT */
    hal_system_speed_init();

    /* Continue basic I/O setup */
    status_green( true );
    status_red( true );

    hal_systick_init();
    hal_adc_init();

}

/* ----- End ---------------------------------------------------------------- */

