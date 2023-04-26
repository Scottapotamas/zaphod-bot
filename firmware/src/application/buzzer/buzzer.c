/* -------------------------------------------------------------------------- */

#include <string.h>

#include "buzzer.h"
#include "hal_pwm.h"
#include "simple_state_machine.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

typedef enum
{
    BUZZER_STATE_OFF,
    BUZZER_STATE_ON,
} BuzzerState_t;

typedef struct
{
    uint16_t      duration;
    uint16_t      frequency;
    uint8_t       count;
} BuzzerRequest_t;

typedef struct
{
    BuzzerState_t previousState;
    BuzzerState_t currentState;
    BuzzerState_t nextState;
    QueueHandle_t xRequestQueue;
    bool          muted;
} Buzzer_t;

PRIVATE Buzzer_t buzzer;
PRIVATE Buzzer_t *me = &buzzer;

/* -------------------------------------------------------------------------- */

PRIVATE void buzzer_process( void );

/* -------------------------------------------------------------------------- */

PUBLIC void
buzzer_init( void )
{
    memset( &buzzer, 0, sizeof( buzzer ) );
    xTaskCreate( buzzer_process, "buzzer", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    // TODO: Massively reduce buzzer priority
}

/* -------------------------------------------------------------------------- */

PUBLIC void
buzzer_sound( uint8_t count, uint16_t frequency, uint16_t duration_ms )
{
    if( !me->muted )
    {
        BuzzerRequest_t new_request = { 0 };

        new_request.count     = count;
        new_request.duration  = duration_ms;
        new_request.frequency = frequency;

        xQueueSendToBack( me->xRequestQueue, ( void * ) &new_request, ( TickType_t ) 0 );
    }
}

/* -------------------------------------------------------------------------- */

PUBLIC void
buzzer_mute( bool muted )
{
    me->muted = muted;
}

/* -------------------------------------------------------------------------- */

PRIVATE void
buzzer_process( void )
{
    // Create a buzzer event request queue
    me->xRequestQueue = xQueueCreate( 10, sizeof(BuzzerRequest_t) );
    REQUIRE( me->xRequestQueue );
    vQueueAddToRegistry( me->xRequestQueue, "beepReq");  // Debug view annotation

    for(;;)
    {
        // Wait indefinitely for an inbound request
        BuzzerRequest_t request = { 0 };
        xQueueReceive( me->xRequestQueue, &request, portMAX_DELAY);
        REQUIRE( request.frequency );

        STATE_INIT_INITIAL( BUZZER_STATE_ON );

        // Alternate between sound/silence until requested number of beeps has been played
        while( request.count > 0 )
        {
            switch( me->currentState )
            {
                case BUZZER_STATE_ON:
                    STATE_ENTRY_ACTION
                        hal_pwm_generation( _PWM_TIM_BUZZER, request.frequency );
                        hal_pwm_set_percentage_f( _PWM_TIM_BUZZER, 50 );
                    STATE_TRANSITION_TEST
                        vTaskDelay( pdMS_TO_TICKS( request.duration ) );
                    STATE_NEXT( BUZZER_STATE_OFF );
                    STATE_EXIT_ACTION
                        hal_pwm_set_percentage_f( _PWM_TIM_BUZZER, 0 );
                    STATE_END
                    break;

                case BUZZER_STATE_OFF:
                    STATE_ENTRY_ACTION
                        request.count--;
                    STATE_TRANSITION_TEST
                        vTaskDelay( pdMS_TO_TICKS( request.duration ) );
                        if( request.count )
                        {
                            STATE_NEXT( BUZZER_STATE_ON );
                        }
                    STATE_EXIT_ACTION
                    STATE_END
                    break;
            }
        }
    }   // infinite loop
}

/* -------------------------------------------------------------------------- */