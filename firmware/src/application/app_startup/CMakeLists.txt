message("Processing application/app_startup")

add_library (
        app_startup_lib
        app_startup.h
        app_startup.c
)

target_include_directories (
        app_startup_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(app_startup_lib
        PRIVATE
        utility_ordering_pool_lib
        guidance_lib
        lighting_lib
        kinematics_lib
        overwatch_lib
        fan_lib
        buzzer_lib
        led_lib
        telemetry_lib
        sensors_lib
        servo_lib
        hal_system_lib
        hal_gpio_lib
        FreeRTOS::Timers
        FreeRTOS::Heap::4
        FreeRTOS::ARM_CM4F
        CMSIS::STM32::F429xx
        )