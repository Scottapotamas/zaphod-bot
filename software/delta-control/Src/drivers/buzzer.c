/* ----- System Includes ---------------------------------------------------- */

#include <string.h>

/* ----- Local Includes ----------------------------------------------------- */

#include "buzzer.h"
#include "simple_state_machine.h"
#include "hal_pwm.h"
#include "hal_systick.h"

/* ----- Private Types ------------------------------------------------------ */

typedef enum
{
    BUZZER_STATE_OFF,
    BUZZER_STATE_ON,
} BuzzerState_t;

typedef struct
{
    BuzzerState_t   previousState;
    BuzzerState_t   currentState;
    BuzzerState_t   nextState;
    uint8_t         count;
    uint16_t        duration;
    uint16_t        frequency;
    uint32_t        timer;
} Buzzer_t;

/* ----- Private Variables -------------------------------------------------- */

PRIVATE Buzzer_t buzzer;

/* ----- Public Functions --------------------------------------------------- */

PUBLIC void
buzzer_init( void )
{
    memset( &buzzer, 0, sizeof( buzzer ) );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
buzzer_sound( uint8_t count, uint16_t frequency, uint16_t duration_ms )
{
    Buzzer_t *me = &buzzer;

    me->count     = count;
    me->duration  = duration_ms;
    me->frequency = frequency;

    STATE_INIT_INITIAL( BUZZER_STATE_ON );
}

/* -------------------------------------------------------------------------- */

PUBLIC void
buzzer_process( void )
{
    Buzzer_t *me = &buzzer;

    switch( me->currentState )
    {
        case BUZZER_STATE_OFF:
            STATE_ENTRY_ACTION

                if( buzzer.count > 0 )
                {
                    buzzer.count--;
                }

                buzzer.timer = hal_systick_get_ms();

            STATE_TRANSITION_TEST

                if( ( buzzer.count > 0 )
                    && ( hal_systick_get_ms() - buzzer.timer ) > buzzer.duration )
                {
                    STATE_NEXT( BUZZER_STATE_ON );
                }

            STATE_EXIT_ACTION
            STATE_END
            break;

        case BUZZER_STATE_ON:
            STATE_ENTRY_ACTION

                buzzer.timer = hal_systick_get_ms();
            	hal_pwm_setup( _PWM_TIM_BUZZER, buzzer.frequency, 50 );

            STATE_TRANSITION_TEST

                if( ( hal_systick_get_ms() - buzzer.timer ) > buzzer.duration )
                {
                    STATE_NEXT( BUZZER_STATE_OFF );
                }

            STATE_EXIT_ACTION

        		hal_pwm_setup( _PWM_TIM_BUZZER, 0, 0 );

            STATE_END
            break;
    }
}

/* ----- End ---------------------------------------------------------------- */
