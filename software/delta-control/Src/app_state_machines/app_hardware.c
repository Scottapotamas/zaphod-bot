/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "hal_systick.h"
#include "hal_dma.h"
#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_soft_ic.h"
#include "hal_watchdog.h"
#include "hal_system_speed.h"

#include "status.h"
#include "configuration.h"
#include "buzzer.h"
#include "fan.h"
#include "sensors.h"
#include "clearpath.h"

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
    hal_soft_ic_init();	//todo get hardware timer input capture working on tim9 for fan speed

	configuration_init();
    buzzer_init();
    fan_init();
    sensors_init();
	sensors_enable();

    //delta main servo motor handlers
    servo_init( _CLEARPATH_1 );
    servo_init( _CLEARPATH_2 );
    servo_init( _CLEARPATH_3 );

#ifdef EXPANSION_SERVO
    servo_init( _CLEARPATH_4 );
#endif

}

/* ----- End ---------------------------------------------------------------- */
