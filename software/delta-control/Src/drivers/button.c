/* ----- Local Includes ----------------------------------------------------- */

#include "button.h"
#include "global.h"
#include "qassert.h"

#include "app_times.h"
#include "hal_systick.h"

/* ----- Defines ------------------------------------------------------------ */

/* ~~~ Button State Machine Handling Macros ~~~ */

#define STATE_INIT_INITIAL(s)           b->previousState = -1; b->currentState = (s); b->nextState = (s)
#define STATE_ENTRY_ACTION              if ( b->currentState != b->previousState ) {
#define STATE_TRANSITION_TEST           b->previousState = b->currentState; } else if ( b->nextState == b->currentState ) {
#define STATE_EXIT_ACTION               } else if ( b->nextState != b->currentState ) {
#define STATE_END                       b->currentState = b->nextState; }
#define STATE_NEXT(s)                   b->nextState = (s)

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
    uint32_t        StartTime;       /*!< Time when button was pressed */
    uint8_t         lastStatus;      /*!< Button status on last check */
    ButtonState_t   previousState;   /*!< Previous button state */
    ButtonState_t   currentState;    /*!< Current button state */
    ButtonState_t   nextState;       /*!< Next button state */
    ButtonHandler_t handler;         /*!< Button function handler */
} Button_t;

PRIVATE Button_t    button[BUTTON_MAX] = { 0 }; /* Ensure clear at startup */

/* ----- Public Functions --------------------------------------------------- */

/* Configure the given button and set the call back handler.
 * Return true when button is already pressed. */

PUBLIC bool
button_init( ButtonId_t id, ButtonHandler_t handler )
{
    Button_t *b = &button[id];

    CRITICAL_SECTION_VAR();
    CRITICAL_SECTION_START();

    b->handler = handler;

    bool pressed = hal_button_is_pressed(id);

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
        Button_t *b = &button[i];

        /* Avoid running this when there is no handler present.
         * e.g. the timer tick may be running before we are fully
         * Initialised
         */
        if( b->handler )
        {
            bool is_pressed = hal_button_is_pressed(i);

            switch( b->currentState )
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
                        b->StartTime = hal_systick_get_ms();
                        if( b->handler )
                        {
                            b->handler( i, BUTTON_PRESS_TYPE_DOWN );
                        }
                    STATE_TRANSITION_TEST
                        if( is_pressed )    /* Button still pressed */
                        {
                            if( (hal_systick_get_ms() - b->StartTime) > TIME_BUTTON_LONG_PRESS )
                            {
                                b->handler( i, BUTTON_PRESS_TYPE_LONG );
                                STATE_NEXT( BUTTON_STATE_WAITRELEASE );
                            }
                        }
                        else if( ! is_pressed ) /* Button was released */
                        {
                            if( (hal_systick_get_ms() - b->StartTime) > TIME_BUTTON_NORMAL_PRESS )
                            {
                                /* Pressed longer than the minimum time */
                                b->handler( i, BUTTON_PRESS_TYPE_NORMAL );
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
                        if( ! is_pressed )
                        {
                            STATE_NEXT( BUTTON_STATE_START );
                        }
                    STATE_EXIT_ACTION
                        if( b->handler )
                        {
                            b->handler( i, BUTTON_PRESS_TYPE_UP );
                        }
                    STATE_END
                    break;
            }
            /* Save current status */
            b->lastStatus = is_pressed;
        }
    }
}

/* ----- End ---------------------------------------------------------------- */
