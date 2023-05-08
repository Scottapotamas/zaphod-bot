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
#include "user_interface.h"
#include "overwatch.h"

#include "effector.h"
#include "servo.h"

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

    user_interface_init();
    Subject *ui_request_subject = user_interface_get_request_subject();

    effector_init();

    servo_init( _CLEARPATH_1 );
    servo_init( _CLEARPATH_2 );
    servo_init( _CLEARPATH_3 );

    overwatch_init();
    Subject *overwatch_commands = overwatch_get_subject();

    // Attach observers to subjects as needed
    subject_add_observer( ui_request_subject, overwatch_get_observer() );

    sensors_add_observer( fan_get_observer() );
    sensors_add_observer( user_interface_get_sensor_observer() );

    // All servos need inbound sensor data, commands, and output state updates
    for( ClearpathServoInstance_t instance = 0; instance < _NUMBER_CLEARPATH_SERVOS; instance++ )
    {
        sensors_add_observer( servo_get_observer(instance) );
        subject_add_observer( overwatch_commands, servo_get_observer(instance) );

        // Overwatch, telemetry tasks want status updates from the servos
        subject_add_observer( servo_get_subject( instance ), overwatch_get_observer() );
        subject_add_observer( servo_get_subject( instance ), user_interface_get_sensor_observer() );

    }

    // TODO other setup
    //   Setup LED instances
    //   Setup guidance/kinematics instances
}

/* -------------------------------------------------------------------------- */

void app_startup_tasks( void )
{
    xTaskCreate( sensors_task, "sensors", configMINIMAL_STACK_SIZE, NULL, priority_low, NULL );
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

    // TODO implement tasks for:
    //   Guidance
    //   RGB LED control
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