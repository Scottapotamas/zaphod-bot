message("Processing drivers/led")

add_library (
        led_lib
        led.h
        led.c
)

target_include_directories (
        led_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        led_lib
        PRIVATE
        hal_gpio_lib
        hal_pwm_lib
        utility_lighting_lib
        utility_observer_lib
        FreeRTOS::ARM_CM4F
        STM32::F4
)