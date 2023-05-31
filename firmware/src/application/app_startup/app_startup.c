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

#include "subject.h"

/* -------------------------------------------------------------------------- */

const uint8_t priority_lowest = tskIDLE_PRIORITY + 1;
const uint8_t priority_low = priority_lowest + 1;
const uint8_t priority_normal = priority_low + 1;
const uint8_t priority_high = priority_normal + 1;
const uint8_t priority_highest = priority_high + 1;

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

    sensors_init();
    fan_init();
    buzzer_init();
    led_init();
    shutter_init();

    request_handler_init( REQUEST_HANDLER_MOVES );
    request_handler_init( REQUEST_HANDLER_FADES );

    path_interpolator_init();
    Subject *pathing_events = path_interpolator_get_subject();

    point_follower_init();

    led_interpolator_init();

    user_interface_init();
    Subject *ui_request_subject = user_interface_get_request_subject();

    effector_init();
    Subject *effector_data = effector_get_subject();

    overwatch_init();
    Subject *overwatch_commands = overwatch_get_subject();

    // Attach observers to subjects as needed

    // Overwatch subscriptions
    subject_add_observer( ui_request_subject, overwatch_get_observer() );
    subject_add_observer( pathing_events, overwatch_get_observer() );
    subject_add_observer( effector_data, overwatch_get_observer() );

    // Shutter release subscribes to overwatch and UI requests
    subject_add_observer( ui_request_subject, shutter_get_observer() );
    subject_add_observer( overwatch_commands, shutter_get_observer() );

    // LED task subscriptions
    subject_add_observer( effector_data, led_get_observer() );

    // Telemetry task wants to know pretty much everything
    sensors_add_observer( user_interface_get_observer() );
    subject_add_observer( overwatch_commands, user_interface_get_observer() );
    subject_add_observer( effector_data, user_interface_get_observer() );
    subject_add_observer( pathing_events, user_interface_get_observer() );

    subject_add_observer( request_handler_get_subject_for(REQUEST_HANDLER_MOVES), user_interface_get_observer() );
    subject_add_observer( request_handler_get_subject_for(REQUEST_HANDLER_FADES), user_interface_get_observer() );

    // Init all servos, setup inbound sensor data, commands, and output state updates
    for( ClearpathServoInstance_t instance = _CLEARPATH_1; instance < _CLEARPATH_4; instance++ )    // _NUMBER_CLEARPATH_SERVOS
    {
        servo_init( instance );

        sensors_add_observer( servo_get_observer(instance) );
        subject_add_observer( overwatch_commands, servo_get_observer(instance) );
        subject_add_observer( effector_data, servo_get_observer(instance) );

        // Add overwatch and telemetry tasks to servo output subjects
        subject_add_observer( servo_get_subject( instance ), overwatch_get_observer() );
        subject_add_observer( servo_get_subject( instance ), user_interface_get_observer() );
    }

    // Misc
    sensors_add_observer( fan_get_observer() );

}

/* -------------------------------------------------------------------------- */

void app_startup_tasks( void )
{
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