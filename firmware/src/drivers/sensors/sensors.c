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

#include "broker.h"
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
    uint32_t timestamp;
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


/* -------------------------------------------------------------------------- */

PUBLIC void sensors_init( void )
{
    // Set up a queue to accept inbound readings from HAL
    xHalQueue = xQueueCreate( 20, sizeof(HalInput_t) );
    REQUIRE( xHalQueue );
    vQueueAddToRegistry( xHalQueue, "sensors");  // Debug view annotation

    // Prepare our averaging structures
    for( HalInputType_t i = 0; i < HAL_NUM_FIELDS; i++ )
    {
        uint8_t span = 16;

        if( i >= IC_SERVO_1_HLFB && i <= IC_SERVO_4_HLFB )
        {
            // HLFB values shouldn't be filtered like this
            span = 1;
        }

        // TODO cleanup the timespan to run averaging on
        average_short_init( &data[i].stats, span );
    }

    // Provide ADC counts -> floating SI units converter functions for each signal
    data[ADC_SERVO_1_CURRENT].converter = hal_current_A;
    data[ADC_SERVO_2_CURRENT].converter = hal_current_A;
    data[ADC_SERVO_3_CURRENT].converter = hal_current_A;
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
        PublishedEvent signal;

        signal.data.stamped.timestamp = new_data.timestamp;

        if( data[new_data.type].converter )
        {
            signal.data.stamped.value.f32 = data[new_data.type].converter(average);
        }
        else
        {
            signal.data.stamped.value.u32 = average;
        }

        // Notify upstream observers of the new data
        signal.topic = new_data.type;
        broker_publish( &signal );

        // Publish additional pre-transformed 'servo power' values
        if( new_data.type <= ADC_SERVO_4_CURRENT )
        {
            float hv_voltage = data[ADC_VOLTAGE_INPUT].converter( average_short_get_average( &data[ADC_VOLTAGE_INPUT].stats ) );

            // Assumptions:
            //  - current reading already converted to amps,
            //  - The current reading timestamp should be used
            //  - Power topics are in same, sequential order as current

            // Apply voltage calc to get power, publish it.
            signal.data.stamped.value.f32 *= hv_voltage;
            signal.topic = new_data.type + SERVO_1_POWER;
            broker_publish( &signal );
        }

    }
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
    new.timestamp = xTaskGetTickCount();

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendToBackFromISR( xHalQueue, (void *)&new, &xHigherPriorityTaskWoken );
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

/* -------------------------------------------------------------------------- */

// Called by the HAL HARD IC layer as part of timer interrupt handling
PRIVATE void sensors_callback_input_capture( InputCaptureSignal_t flag, uint32_t value )
{
    HalInput_t new = { 0 };
    new.type = (HalInputType_t)flag + IC_SERVO_1_HLFB;    // 'translate' ic enum values into the sensor enum range
    new.value = value;
    new.timestamp = xTaskGetTickCount();

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendToBackFromISR( xHalQueue, (void *)&new, &xHigherPriorityTaskWoken );
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

/* -------------------------------------------------------------------------- */