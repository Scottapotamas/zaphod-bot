/* -------------------------------------------------------------------------- */

#include "shutter_release.h"
#include "qassert.h"

#include "FreeRTOS.h"
#include "timers.h"

#include "hal_gpio.h"

/* -------------------------------------------------------------------------- */

DEFINE_THIS_FILE;

/* -------------------------------------------------------------------------- */

PRIVATE Observer events = { 0 };

PRIVATE TimerHandle_t xShutterReleaseTimer;

/* -------------------------------------------------------------------------- */

PRIVATE void system_events_callback( ObserverEvent_t event, EventData eData, void *context );

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
    observer_init( &events, system_events_callback, NULL );

    observer_subscribe( &events, FLAG_REQUEST_SHUTTER_RELEASE );
    observer_subscribe( &events, FLAG_ESTOP );
    observer_subscribe( &events, FLAG_REQUEST_QUEUE_CLEAR );
}

/* -------------------------------------------------------------------------- */

PUBLIC Observer * shutter_get_observer( void )
{
    return &events;
}

/* -------------------------------------------------------------------------- */

PRIVATE void system_events_callback( ObserverEvent_t event,
                                     EventData eData,
                                     void *context )
{
    switch( event )
    {
        case FLAG_REQUEST_SHUTTER_RELEASE:
            if( eData.stamped.data.u32 == 0 )
            {
                shutter_cancel_capture();
            }
            else
            {
                shutter_begin_capture( eData.stamped.data.u32 );
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