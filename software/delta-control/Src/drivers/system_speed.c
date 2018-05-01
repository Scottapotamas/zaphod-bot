/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "system_speed.h"
#include "simple_state_machine.h"
#include "hal_system_speed.h"

#include "hal_systick.h"
#include "hal_uart.h"
#include "hal_gpio.h"
#include "stm32f4xx_hal.h"

/* ----- Private Types ------------------------------------------------------ */

typedef enum
{
   SYSTEM_SPEED_STATE_LOW,
   SYSTEM_SPEED_STATE_HIGH
} SystemSpeedStates_t;

typedef struct
{
    SystemSpeedStates_t previousState;
    SystemSpeedStates_t currentState;
    SystemSpeedStates_t nextState;
    uint32_t            speed_map;
} SystemSpeedStateVars_t;

/* ----- Private Data ------------------------------------------------------- */

PRIVATE SystemSpeedStateVars_t system_speed_state_machine;
PRIVATE SystemSpeedStateVars_t *me = &system_speed_state_machine;

/* ----- Public Functions --------------------------------------------------- */

/** Init the system clock to a default level */

PUBLIC void
system_speed_init( void )
{
    memset( &system_speed_state_machine, 0, sizeof(system_speed_state_machine) );

    hal_system_speed_init();

    STATE_INIT_INITIAL( SYSTEM_SPEED_STATE_LOW );
    system_speed_process();
}

/* -------------------------------------------------------------------------- */

/** Set the system clock speed to a different level */

PUBLIC void
system_speed_set( uint8_t id, SystemSpeed_t speed )
{
    switch( speed )
    {
        case SYSTEM_SPEED_FULL:
            me->speed_map = _BV(id);
            break;

        case SYSTEM_SPEED_LOW:
            me->speed_map &= ~_BV(id);
            break;
    }
    system_speed_process();
}


/* -------------------------------------------------------------------------- */

PUBLIC SystemSpeed_t
system_speed_get( void )
{
    return me->speed_map ? SYSTEM_SPEED_FULL : SYSTEM_SPEED_LOW;
}

/* -------------------------------------------------------------------------- */

/** Process the system clock speed setting */

PUBLIC void
system_speed_process( void )
{
    do
    {
        switch( me->currentState )
        {
           case SYSTEM_SPEED_STATE_LOW:
               STATE_ENTRY_ACTION
                   me->speed_map = 0;
                   hal_system_speed_low();
                   hal_systick_init();

               STATE_TRANSITION_TEST
                    if( me->speed_map != 0 )
                    {
                        STATE_NEXT( SYSTEM_SPEED_STATE_HIGH );
                    }
                STATE_EXIT_ACTION
                STATE_END
                break;

           case SYSTEM_SPEED_STATE_HIGH:
               STATE_ENTRY_ACTION
                   hal_system_speed_high();
                   hal_systick_init();

                STATE_TRANSITION_TEST
                    if( me->speed_map == 0 )
                    {
                        STATE_NEXT( SYSTEM_SPEED_STATE_LOW );
                    }
                STATE_EXIT_ACTION
                STATE_END
                break;
        }
    } while( STATE_IS_TRANSITIONING );
}

/* ----- End ---------------------------------------------------------------- */

