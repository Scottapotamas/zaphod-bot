/* ----- System Includes ---------------------------------------------------- */


/* ----- Local Includes ----------------------------------------------------- */

#include "app_background.h"
#include "event_subscribe.h"
#include "app_signals.h"
#include "button.h"
#include "hal_button.h"
#include "timer_ms.h"

/* -------------------------------------------------------------------------- */

PRIVATE timer_ms_t button_timer = 0;

/* -------------------------------------------------------------------------- */

PUBLIC void
app_background_init( void )
{
	timer_ms_start( &button_timer, 50 );

}

/* -------------------------------------------------------------------------- */

PUBLIC void
app_background( void )
{
    if( timer_ms_is_expired( &button_timer ) )
    {
    	timer_ms_start( &button_timer, 50 );
    }

    /* Scan buttons */
    button_process();

}

/* ----- End ---------------------------------------------------------------- */

