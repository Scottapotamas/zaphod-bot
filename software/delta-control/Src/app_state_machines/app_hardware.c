/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "hal_systick.h"
#include "hal_dma.h"
#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_watchdog.h"
#include "hal_system_speed.h"
#include "status.h"

/* -------------------------------------------------------------------------- */

PUBLIC void
app_hardware_init( void )
{
    /* Configure GPIO pins */
    hal_gpio_configure_defaults();

    /* Start the watchdog for 20s timeout */
    hal_watchdog_init( 20000 );

    /* Initialise the CPU manager DWT */
    hal_system_speed_init();

    /* Continue basic I/O setup */
    status_green( 	true );
    status_yellow( 	true );
    status_red( 	true );

    hal_systick_init();
    hal_dma_init();
    hal_adc_init();
}

/* ----- End ---------------------------------------------------------------- */
