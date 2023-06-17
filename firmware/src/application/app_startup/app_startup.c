/* -------------------------------------------------------------------------- */

#include "app_startup.h"
#include "FreeRTOS.h"
#include "task.h"

#include "hal_core.h"
#include "hal_reset.h"
#include "hal_gpio.h"

#include "sensors.h"
#include "fan.h"
#include "buzzer.h"
#include "shutter_release.h"
#include "user_interface.h"
#include "overwatch.h"

#include "request_handler.h"

#include "path_interpolator.h"
#include "point_follower.h"
#include "effector.h"
#include "servo.h"

#include "led_interpolator.h"
#include "led.h"

#include "broker.h"

/* -------------------------------------------------------------------------- */

const uint8_t priority_lowest = tskIDLE_PRIORITY + 1;
const uint8_t priority_low = priority_lowest + 1;
const uint8_t priority_normal = priority_low + 1;
const uint8_t priority_high = priority_normal + 1;
const uint8_t priority_highest = priority_high + 1;

Broker pubsub_broker = { 0 };

/* -------------------------------------------------------------------------- */

void app_startup_init( void )
{
    hal_core_init();
    hal_core_clock_configure();
    hal_gpio_configure_defaults();

    // TODO: Watchdog configuration

    // Check for the cause of the microcontroller booting (errors vs normal power up)
    user_interface_set_reset_cause( hal_reset_cause_description( hal_reset_cause() ) );
    user_interface_set_assert_cause( hal_reset_assert_description() );

    broker_init( &pubsub_broker );

    sensors_init();
    fan_init();
    buzzer_init();
    led_init();
    shutter_init();

    request_handler_init( REQUEST_HANDLER_MOVES );
    request_handler_init( REQUEST_HANDLER_FADES );

    path_interpolator_init();
    point_follower_init();
    led_interpolator_init();

    user_interface_init();
    effector_init();
    overwatch_init();

    // Init all servos
    for( ClearpathServoInstance_t instance = _CLEARPATH_1; instance < _CLEARPATH_4; instance++ )    // _NUMBER_CLEARPATH_SERVOS
    {
        servo_init( instance );
    }

}

/* -------------------------------------------------------------------------- */

void app_startup_tasks( void )
{
    xTaskCreate( broker_task, "pubsub", configMINIMAL_STACK_SIZE, NULL, priority_normal, NULL );

    xTaskCreate( sensors_task, "sensors", configMINIMAL_STACK_SIZE+200, NULL, priority_low, NULL );
    xTaskCreate( buzzer_task, "buzzer", configMINIMAL_STACK_SIZE, NULL, priority_lowest, NULL );
    xTaskCreate( fan_task, "fan", configMINIMAL_STACK_SIZE, NULL, priority_lowest, NULL );

    xTaskCreate( user_interface_task,
                 "telemetry",
                 4000,
                 NULL,
                 priority_low,
                 NULL
    );

    xTaskCreate( effector_task,
                 "effector",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 priority_normal,
                 NULL
    );

    xTaskCreate( path_interpolator_task,
                 "pathing",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 priority_normal,
                 NULL
    );

    xTaskCreate( point_follower_task,
                 "follower",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 priority_normal,
                 NULL
    );

    xTaskCreate( led_interpolator_task,
                 "lighting",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 priority_normal,
                 NULL
    );

    xTaskCreate( request_handler_task,
                 "rqhMove",
                 configMINIMAL_STACK_SIZE,
                 request_handler_get_context_for(REQUEST_HANDLER_MOVES),
                 priority_low,
                 NULL
    );

    xTaskCreate( request_handler_task,
                 "rqhFade",
                 configMINIMAL_STACK_SIZE,
                 request_handler_get_context_for(REQUEST_HANDLER_FADES),
                 priority_low,
                 NULL
    );

    xTaskCreate( led_task,
                 "led",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 priority_low,
                 NULL
    );

    xTaskCreate( servo_task,
                 "s1",
                 configMINIMAL_STACK_SIZE,
                 servo_get_state_context_for(_CLEARPATH_1),
                 priority_normal,
                 NULL
                 );

    xTaskCreate( servo_task,
                 "s2",
                 configMINIMAL_STACK_SIZE,
                 servo_get_state_context_for(_CLEARPATH_2),
                 priority_normal,
                 NULL
    );
    xTaskCreate( servo_task,
                 "s3",
                 configMINIMAL_STACK_SIZE,
                 servo_get_state_context_for(_CLEARPATH_3),
                 priority_normal,
                 NULL
    );

    xTaskCreate( overwatch_task,
                 "overwatch",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 priority_normal,
                 NULL
    );

}

/* -------------------------------------------------------------------------- */