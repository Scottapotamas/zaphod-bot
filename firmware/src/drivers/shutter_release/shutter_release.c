/* -------------------------------------------------------------------------- */

#include "shutter_release.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "timers.h"

#include "hal_gpio.h"

#include "broker.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

PRIVATE TimerHandle_t xShutterReleaseTimer;

PRIVATE Subscriber *event_sub = { 0 };

/* -------------------------------------------------------------------------- */

PRIVATE void shutter_release_active( bool on );

PRIVATE void shutter_timer_callback( TimerHandle_t xTimer );

PRIVATE void shutter_begin_capture( uint32_t timeout_ms );

PRIVATE void shutter_cancel_capture( void );

/* -------------------------------------------------------------------------- */

PUBLIC void shutter_init( void )
{
    // Setup a stimulus timer handle
    xShutterReleaseTimer = xTimerCreate("shutter_release",
                                            pdMS_TO_TICKS(100),
                                            pdFALSE,
                                            0,
                                            shutter_timer_callback
    );
    REQUIRE( xShutterReleaseTimer );

    // Setup subscriptions to events
    event_sub = broker_create_subscriber( "psShutter", 3 );
    REQUIRE( event_sub );

    broker_add_event_subscription( event_sub, FLAG_REQUEST_SHUTTER_RELEASE );
    broker_add_event_subscription( event_sub, FLAG_ESTOP );
    broker_add_event_subscription( event_sub, FLAG_REQUEST_QUEUE_CLEAR );
}

/* -------------------------------------------------------------------------- */

PUBLIC void shutter_task( void* arg )
{
    PublishedEvent event = { 0 };

    for(;;)
    {
        // Block on relevant events
        xQueueReceive( event_sub->queue, &event, portMAX_DELAY );

        switch( event.topic )
        {
            case FLAG_REQUEST_SHUTTER_RELEASE:
                if( event.data.stamped.value.u32 == 0 )
                {
                    shutter_cancel_capture();
                }
                else
                {
                    shutter_begin_capture( event.data.stamped.value.u32 );
                }
            break;

            case FLAG_REQUEST_QUEUE_CLEAR:
            case FLAG_ESTOP:
                shutter_cancel_capture();
                break;

            default:
                ASSERT(false);
                break;
        }

    }
}

/* -------------------------------------------------------------------------- */

PRIVATE void shutter_release_active( bool on )
{
    hal_gpio_write_pin( _EXT_OUTPUT_1, on );
}

/* -------------------------------------------------------------------------- */

PRIVATE void shutter_timer_callback( TimerHandle_t xTimer )
{
    shutter_release_active( false );
}

PRIVATE void shutter_begin_capture( uint32_t timeout_ms )
{
    REQUIRE( xShutterReleaseTimer );
    xTimerChangePeriod( xShutterReleaseTimer, pdMS_TO_TICKS(timeout_ms), 1 );
    xTimerStart( xShutterReleaseTimer, 1 );

    shutter_release_active( true );
}

PRIVATE void shutter_cancel_capture( void )
{
    REQUIRE( xShutterReleaseTimer );
    xTimerStop( xShutterReleaseTimer, 2 );

    shutter_release_active( false );
}

/* -------------------------------------------------------------------------- */