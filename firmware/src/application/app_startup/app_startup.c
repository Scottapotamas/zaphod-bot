/* -------------------------------------------------------------------------- */

#include "app_startup.h"
#include "FreeRTOS.h"
#include "task.h"

#include "hal_core.h"
#include "hal_gpio.h"

#include "sensors.h"
#include "fan.h"
#include "buzzer.h"
#include "user_interface.h"

/* -------------------------------------------------------------------------- */

void app_startup_init( void )
{
    hal_core_init();
    hal_core_clock_configure();
    hal_gpio_configure_defaults();

    // TODO: Init remaining system functionality
    //  Reset cause check
    //  Watchdog configuration

    // Check for the cause of the microcontroller booting (errors vs normal power up)
    //    user_interface_set_reset_cause( hal_reset_cause_description( hal_reset_cause() ) );
    //    user_interface_set_assert_cause( hal_reset_assert_description() );

    sensors_init();
    fan_init();
    buzzer_init();
    user_interface_init();

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
    // TODO: Fix task priorities
    xTaskCreate( sensors_task, "sensors", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( buzzer_task, "buzzer", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
    xTaskCreate( fan_task, "fan", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

    xTaskCreate( user_interface_task,
                 "telemetry",
                 4000,
                 NULL,
                 tskIDLE_PRIORITY + 1,
                 NULL
    );

    // TODO implement tasks for:
    //   Guidance
    //   Kinematics handling
    //   RGB LED control

}

/* -------------------------------------------------------------------------- */