/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "shutter_release.h"
#include "simple_state_machine.h"
#include "hal_systick.h"
#include "app_times.h"
#include "hal_gpio.h"

/* ----- Private Types ------------------------------------------------------ */

typedef enum
{
    SHUTTER_STATE_OFF,
    SHUTTER_STATE_ON,
} ShutterState_t;

typedef struct
{
    ShutterState_t   previousState;
    ShutterState_t   currentState;
    ShutterState_t   nextState;

    HalGpioPortPin_t control_pin;       // the GPIO responsible for triggering the camera
    uint32_t        exposure_timer;	    // track how long the shutter has been open for
    uint32_t        exposure_duration;  // amount of time to hold the shutter release line for
    bool            capture;
} Shutter_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Shutter_t shutter;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
shutter_init( void )
{
    memset( &shutter, 0, sizeof( shutter ) );
    shutter.control_pin = _EXT_OUTPUT_1;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
shutter_capture( uint32_t duration_ms )
{
	Shutter_t *me = &shutter;

	// set the time to expose for, and kick into the exposure state
    me->exposure_duration = duration_ms;
    me->capture = true;
}

PUBLIC void
shutter_cancel( void )
{
    Shutter_t *me = &shutter;
    me->capture = false;
}

/* -------------------------------------------------------------------------- */

PUBLIC void
shutter_process( void )
{
	Shutter_t *me = &shutter;

    switch( me->currentState )
    {
        case SHUTTER_STATE_OFF:
            STATE_ENTRY_ACTION

                hal_gpio_write_pin( me->control_pin, GPIO_LOW);

            STATE_TRANSITION_TEST

                if(  me->capture )
                {
                    STATE_NEXT( SHUTTER_STATE_ON );
                }

            STATE_EXIT_ACTION
            STATE_END
            break;

        case SHUTTER_STATE_ON:
            STATE_ENTRY_ACTION

                hal_gpio_write_pin( me->control_pin , GPIO_HIGH );
                me->exposure_timer = hal_systick_get_ms();

        STATE_TRANSITION_TEST

                if( ( hal_systick_get_ms() - me->exposure_timer ) > me->exposure_duration || !me->capture )
                {
                    STATE_NEXT( SHUTTER_STATE_OFF );
                }

            STATE_EXIT_ACTION

                hal_gpio_write_pin( me->control_pin, GPIO_LOW );
                me->capture = false;

            STATE_END
            break;
    }

}

/* ----- End ---------------------------------------------------------------- */
