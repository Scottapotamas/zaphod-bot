/* -------------------------------------------------------------------------- */

#include "sensors.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"

#include "hal_adc.h"
#include "hal_power.h"
#include "hal_temperature.h"
#include "hal_ic_hard.h"
#include "hal_fan_tacho.h"
#include "hal_servo_hlfb.h"

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

typedef enum {

    // ADC backed events
    // Don't re-order
    ADC_SERVO_1_CURRENT = 0,
    ADC_SERVO_2_CURRENT,
    ADC_SERVO_3_CURRENT,
    ADC_SERVO_4_CURRENT,

    ADC_VOLTAGE_INPUT,
    ADC_TEMPERATURE_PCB,
    ADC_TEMPERATURE_REGULATOR,
    ADC_TEMPERATURE_EXTERNAL,
    ADC_TEMPERATURE_MICRO,
    ADC_VOLTAGE_INTERNAL_REF,

    // Input capture backed events
    // Don't re-order
    IC_SERVO_1_HLFB,
    IC_SERVO_2_HLFB,
    IC_SERVO_3_HLFB,
    IC_SERVO_4_HLFB,
    IC_FAN_SPEED,

    HAL_NUM_FIELDS,  // Only marks end of enum range, shouldn't exceed 64 entries
} HalInputType_t;

typedef struct
{
    HalInputType_t type;
    uint32_t value;
} HalInput_t;

typedef float (*sensor_conversion_fn)(uint32_t adc);

typedef struct {
    uint32_t timestamp;
    AverageShort_t stats;
    sensor_conversion_fn converter;
} FilteredData_t;

FilteredData_t data[HAL_NUM_FIELDS] = { 0 };

QueueHandle_t xHalQueue;
TimerHandle_t xADCTriggerTimer;

Subject subject = { 0 };

/* -------------------------------------------------------------------------- */

PUBLIC void sensors_init( void )
{
    // Create a subject
    subject_init(&subject);

    // Set up a queue to accept inbound readings from HAL
    xHalQueue = xQueueCreate( 20, sizeof(HalInput_t) );
    REQUIRE( xHalQueue );
    vQueueAddToRegistry( xHalQueue, "sensors");  // Debug view annotation

    // Prepare our averaging structures
    for( uint32_t i = 0; i < HAL_NUM_FIELDS; i++ )
    {
        // TODO cleanup the timespan to run averaging on
        average_short_init( &data[i].stats, 8 );
    }

    // Provide ADC counts -> floating SI units converter functions for each signal
    data[ADC_SERVO_1_CURRENT].converter = hal_current_A;
    data[ADC_SERVO_2_CURRENT].converter = hal_current_A;
    data[ADC_SERVO_4_CURRENT].converter = hal_current_A;
    data[ADC_VOLTAGE_INPUT].converter = hal_current_A;

    data[ADC_VOLTAGE_INPUT].converter = hal_voltage_V;

    data[ADC_TEMPERATURE_PCB].converter = hal_temperature_pcb_degrees_C;
    data[ADC_TEMPERATURE_REGULATOR].converter = hal_temperature_pcb_degrees_C;
    data[ADC_TEMPERATURE_EXTERNAL].converter = hal_temperature_ext_degrees_C;
    data[ADC_TEMPERATURE_MICRO].converter = hal_temperature_micro_degrees_C;

    data[IC_SERVO_1_HLFB].converter = hal_servo_hlfb;
    data[IC_SERVO_2_HLFB].converter = hal_servo_hlfb;
    data[IC_SERVO_3_HLFB].converter = hal_servo_hlfb;
    data[IC_SERVO_4_HLFB].converter = hal_servo_hlfb;
    data[IC_FAN_SPEED].converter = hal_fan_tacho_rpm;


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

        REQUIRE( new_data.type < HAL_NUM_FIELDS );

        // Maintain stats on the data
        uint16_t average = average_short_update( &data[new_data.type].stats, new_data.value );

        // Prep notification structure, perform unit conversions
        ObserverEvent_t topic;  // event flag to publish
        EventData signal;       // event structure being sent


        if( data[new_data.type].converter )
        {
            signal.data.f32 = data[new_data.type].converter(average);
        }
        else
        {
            signal.data.u32 = average;
        }

        // Handle 'instanced' events
        topic = new_data.type;
        if( new_data.type > ADC_SERVO_4_CURRENT )
        {
            topic -= ADC_SERVO_4_CURRENT;
        }

        if( new_data.type > IC_SERVO_4_HLFB )
        {
            topic -= (IC_SERVO_4_HLFB - IC_SERVO_1_HLFB );
        }

        if( new_data.type >= ADC_SERVO_1_CURRENT && new_data.type <= ADC_SERVO_4_CURRENT )
        {
            topic = SENSOR_SERVO_CURRENT;
            signal.index = new_data.type - ADC_SERVO_1_CURRENT;
        }

        if( new_data.type >= IC_SERVO_1_HLFB && new_data.type <= IC_SERVO_4_HLFB )
        {
            topic = SENSOR_SERVO_HLFB;
            signal.index = new_data.type - IC_SERVO_1_HLFB;
        }

        // Notify upstream observers
        subject_notify(&subject, topic, signal );

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
    new.type = (HalInputType_t)flag;
    new.value = value;

    xQueueSendToBackFromISR( xHalQueue, (void *)&new, 0 );
}

/* -------------------------------------------------------------------------- */

// Called by the HAL HARD IC layer as part of timer interrupt handling
PRIVATE void sensors_callback_input_capture( InputCaptureSignal_t flag, uint32_t value )
{
    HalInput_t new = { 0 };
    new.type = (HalInputType_t)flag + IC_SERVO_1_HLFB;    // 'translate' ic enum values into the sensor enum range
    new.value = value;
    xQueueSendToBackFromISR( xHalQueue, (void *)&new, 0 );
}

/* -------------------------------------------------------------------------- */