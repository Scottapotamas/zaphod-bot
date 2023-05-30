message("Processing drivers/status_led")

add_library (
        status_led_lib
        status.h
        status.c
)

target_include_directories (
        status_led_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        status_led_lib
        PRIVATE
        hal_gpio_lib
        utility_observer_lib
        FreeRTOS::ARM_CM4F
        STM32::F4
)