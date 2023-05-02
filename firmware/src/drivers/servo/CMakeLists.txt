message("Processing drivers/servo")

add_library (
        servo_lib
        servo.h
        servo.c
)

target_include_directories (
        servo_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        servo_lib
        PRIVATE
        hal_gpio_lib
        utility_observer_lib
        utility_stopwatch_lib
        FreeRTOS::ARM_CM4F
        # TODO: work out how to compile/link with hard-float without specifying the STM32::F4 package?
        STM32::F4
)