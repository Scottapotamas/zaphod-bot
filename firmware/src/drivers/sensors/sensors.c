/* -------------------------------------------------------------------------- */

#include "sensors.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"

#include "hal_adc.h"
#include "hal_ic_hard.h"
#include "average_short.h"

#include "subject.h"
#include "observer.h"
#include "signals.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

PRIVATE void sensors_trigger_adc_callback( TimerHandle_t xTimer );
PRIVATE void sensors_callback_adc( HalAdcInput_t flag, uint16_t value );
PRIVATE void sensors_callback_input_capture( InputCaptureSignal_t flag, uint32_t value );

/* -------------------------------------------------------------------------- */

typedef struct
{
    SENSOR_EVENT_FLAG type;
    uint16_t value;
} HalInput_t;

typedef struct {
    uint32_t timestamp;
    AverageShort_t stats;
} FilteredData_t;

FilteredData_t data[SENSOR_NUM_FIELDS] = { 0 };

QueueHandle_t xHalQueue;
TimerHandle_t xADCTriggerTimer;

Subject subject = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC void sensors_init( void )
{
    // Create a subject
    subject_init(&subject);

    // Set up a queue to accept inbound readings from HAL
    xHalQueue = xQueueCreate( 50, sizeof(HalInput_t) );
    REQUIRE( xHalQueue );
    vQueueAddToRegistry( xHalQueue, "sensors");  // Debug view annotation

    // Prepare our averaging structures
    for( uint32_t i = 0; i < SENSOR_NUM_FIELDS; i++ )
    {
        // TODO cleanup the timespan to run averaging on
        average_short_init( &data[i].stats, 32 );
    }

    // Configure the hardware peripherals
    hal_adc_init( sensors_callback_adc );
    hal_ic_hard_init( sensors_callback_input_capture );

    // Create a FreeRTOS software timer with a period of 10ms
    // TODO: consider configuring a timer and triggering ADC start on an edge
    xADCTriggerTimer = xTimerCreate("trigADC",
                                         pdMS_TO_TICKS(50),
                                         pdTRUE,
                                         0,
                                         sensors_trigger_adc_callback
                                         );
    REQUIRE( xADCTriggerTimer );
    xTimerStart( xADCTriggerTimer, 0 );
}

/* -------------------------------------------------------------------------- */

PUBLIC void sensors_task( void *arg )
{
    // Data handling/post-processing task
    for(;;)
    {
        // Block on HAL ISR callbacks putting raw data into the queue
        HalInput_t new_data = { 0 };
        xQueueReceive( xHalQueue, &new_data, portMAX_DELAY );

        REQUIRE( new_data.type < SENSOR_NUM_FIELDS );

        // Maintain stats on the data
        uint16_t average = average_short_update( &data[new_data.type].stats, new_data.value );

        // Prep notification structure, perform unit conversions
        EventData event;
        event.uint32Value = average;

        // Notify upstream observers
        subject_notify(&subject, new_data.type, event );

    }

    // Clean up
    //    subject_remove_observer(&subject, &observer1);
    //    subject_remove_observer(&subject, &observer2);
}

/* -------------------------------------------------------------------------- */

// Ability for external modules to add Observers.
// Observers are responsible for their own event subscriptions
PUBLIC void sensors_add_observer( Observer *observer )
{
    subject_add_observer( &subject, observer );
}

/* -------------------------------------------------------------------------- */

// Software timer to trigger an ADC conversion in software
PRIVATE void sensors_trigger_adc_callback( TimerHandle_t xTimer )
{
    hal_adc_start();
}

/* -------------------------------------------------------------------------- */

// Called by the HAL ADC layer as part of DMA interrupt
PRIVATE void sensors_callback_adc( HalAdcInput_t flag, uint16_t value )
{
    HalInput_t new = { 0 };
    new.type = (SENSOR_EVENT_FLAG)flag;
    new.value = value;

    xQueueSendToBackFromISR( xHalQueue, (void *)&new, 0 );
}

/* -------------------------------------------------------------------------- */

// Called by the HAL HARD IC layer as part of timer interrupt handling
PRIVATE void sensors_callback_input_capture( InputCaptureSignal_t flag, uint32_t value )
{
    HalInput_t new = { 0 };
    new.type = (SENSOR_EVENT_FLAG)flag + SENSOR_SERVO_1_HLFB;    // 'translate' ic enum values into the sensor enum range
    new.value = value;
    xQueueSendToBackFromISR( xHalQueue, (void *)&new, 0 );
}

/* -------------------------------------------------------------------------- */