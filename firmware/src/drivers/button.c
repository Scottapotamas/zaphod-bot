/* ----- Local Includes ----------------------------------------------------- */

#include "button.h"
#include "global.h"
#include "qassert.h"

#include "app_times.h"
#include "hal_systick.h"
#include "simple_state_machine.h"

/* ----- Defines ------------------------------------------------------------ */

/* ~~~ Button State Machine Variables ~~~ */

typedef enum
{
    BUTTON_STATE_START,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_WAITRELEASE
} ButtonState_t;

/* Internal structure */
typedef struct
{
    uint32_t        StartTime;     /*!< Time when button was pressed */
    uint8_t         lastStatus;    /*!< Button status on last check */
    ButtonState_t   previousState; /*!< Previous button state */
    ButtonState_t   currentState;  /*!< Current button state */
    ButtonState_t   nextState;     /*!< Next button state */
    ButtonHandler_t handler;       /*!< Button function handler */
} Button_t;

PRIVATE Button_t button[BUTTON_MAX] = { 0 }; /* Ensure clear at startup */

/* ----- Public Functions --------------------------------------------------- */

/* Configure the given button and set the call back handler.
 * Return true when button is already pressed. */

PUBLIC bool
button_init( ButtonId_t id, ButtonHandler_t handler )
{
    Button_t *me = &button[id];

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    me->handler = handler;

    bool pressed = hal_button_is_pressed( id );

    if( pressed )
    {
        STATE_INIT_INITIAL( BUTTON_STATE_WAITRELEASE );
    }
    else
    {
        STATE_INIT_INITIAL( BUTTON_STATE_START );
    }

    CRITICAL_SECTION_END();

    return pressed;
}

/* -------------------------------------------------------------------------- */

PUBLIC bool
button_pattern_match( ButtonPattern_t pattern )
{
    ButtonPattern_t match = 0;
    for( uint8_t i = 0; i < BUTTON_MAX; i++ )
    {
        if( hal_button_is_pressed( i ) )
        {
            match |= _BV( i );
        }
    }

    return pattern == match;
}

/* -------------------------------------------------------------------------- */

/* Background process checking button presses */

PUBLIC void
button_process( void )
{
    for( uint8_t i = 0; i < BUTTON_MAX; i++ )
    {
        Button_t *me = &button[i];

        /* Avoid running this when there is no handler present.
         * e.g. the timer tick may be running before we are fully
         * Initialised
         */
        if( me->handler )
        {
            bool is_pressed = hal_button_is_pressed( i );

            switch( me->currentState )
            {
                case BUTTON_STATE_START:
                    STATE_ENTRY_ACTION
                    STATE_TRANSITION_TEST
                    if( is_pressed )
                    {
                        STATE_NEXT( BUTTON_STATE_PRESSED );
                    }
                    STATE_EXIT_ACTION
                    STATE_END
                    break;

                case BUTTON_STATE_PRESSED:
                    STATE_ENTRY_ACTION
                    me->StartTime = hal_systick_get_ms();
                    if( me->handler )
                    {
                        me->handler( i, BUTTON_PRESS_TYPE_DOWN );
                    }
                    STATE_TRANSITION_TEST
                    if( is_pressed ) /* Button still pressed */
                    {
                        if( ( hal_systick_get_ms() - me->StartTime ) > TIME_BUTTON_LONG_PRESS )
                        {
                            me->handler( i, BUTTON_PRESS_TYPE_LONG );
                            STATE_NEXT( BUTTON_STATE_WAITRELEASE );
                        }
                    }
                    else if( !is_pressed ) /* Button was released */
                    {
                        if( ( hal_systick_get_ms() - me->StartTime ) > TIME_BUTTON_NORMAL_PRESS )
                        {
                            /* Pressed longer than the minimum time */
                            me->handler( i, BUTTON_PRESS_TYPE_NORMAL );
                            STATE_NEXT( BUTTON_STATE_WAITRELEASE );
                        }
                        else
                        {
                            /* Ignore presses that are too short */
                            STATE_NEXT( BUTTON_STATE_WAITRELEASE );
                        }
                    }
                    else
                    {
                        STATE_NEXT( BUTTON_STATE_WAITRELEASE );
                    }
                    STATE_EXIT_ACTION
                    STATE_END
                    break;

                case BUTTON_STATE_WAITRELEASE:
                    STATE_ENTRY_ACTION
                    STATE_TRANSITION_TEST
                    if( !is_pressed )
                    {
                        STATE_NEXT( BUTTON_STATE_START );
                    }
                    STATE_EXIT_ACTION
                    if( me->handler )
                    {
                        me->handler( i, BUTTON_PRESS_TYPE_UP );
                    }
                    STATE_END
                    break;
            }
            /* Save current status */
            me->lastStatus = is_pressed;
        }
    }
}

/* ----- End ---------------------------------------------------------------- */
