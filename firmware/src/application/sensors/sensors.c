#include "sensors.h"
#include "subject.h"
#include "observer.h"

Subject subject = { 0 };
Observer observer1 = { 0 };
Observer observer2 = { 0 };

// Callback functions for two observers
void observer1_callback( EventFlag event,
                         EventData data,
                         void *context )
{
    if( data.floatValue < 0.0f  )
    {
        asm("NOP");
    }
//    printf("Observer 1 received event %d with data %f\n", event, data.floatValue);
}

void observer2_callback( EventFlag event,
                         EventData data,
                         void *context )
{
//    printf("Observer 2 received event %d with data %u\n", event, data.uint32Value);
    if( data.uint32Value > 32 )
    {
        asm("NOP");
    }
}

// Test functions to generate sensor data
float test_float = 3.1415f;
void sensorTask1(void *arg)
{
    for(;;)
    {
        EventData event;
        event.floatValue = test_float;
        test_float -= 1.0f;
        subject_notify(&subject, SENSOR_EVENT_A, event);

        vTaskDelay( pdMS_TO_TICKS(50) );
    }
}

uint32_t testInt = 42;
void sensorTask2(void *arg)
{
    for(;;)
    {
        // Notify observer with sample data
        EventData event;
        event.uint32Value = testInt;
        testInt += 2;
        subject_notify(&subject, SENSOR_EVENT_B, event);

        vTaskDelay( pdMS_TO_TICKS(100) );
    }
}


void sensors_init( void )
{
    // Create a subject
    subject_init(&subject);

    // Create two observers

    observer_init(&observer1, &observer1_callback, NULL);
    observer_init(&observer2, &observer2_callback, NULL);

    // Add observers to the subject
    subject_add_observer(&subject, &observer1);
    subject_add_observer(&subject, &observer2);

    observer_subscribe( &observer1, SENSOR_EVENT_A );
    observer_subscribe( &observer2, SENSOR_EVENT_B );

    // "Sensor data generation"
    xTaskCreate(sensorTask1, "sen1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(sensorTask2, "sen2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Clean up
//    subject_remove_observer(&subject, &observer1);
//    subject_remove_observer(&subject, &observer2);
}

