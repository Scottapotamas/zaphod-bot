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

    effector_init();

    overwatch_init();
    sensors_add_observer( fan_get_observer() );
    sensors_add_observer( user_interface_get_sensor_observer() );

    // TODO other setup
    //   Setup servo instances
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

    // TODO implement tasks for:
    //   Guidance
    //   Kinematics handling
    //   RGB LED control

    xTaskCreate( overwatch_task,
                 "overwatch",
                 configMINIMAL_STACK_SIZE,
                 NULL,
                 priority_normal,
                 NULL
    );

}

/* -------------------------------------------------------------------------- */