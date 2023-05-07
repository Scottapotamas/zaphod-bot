message("Processing drivers/fan")

add_library (
        fan_lib
        fan.h
        fan.c
)

target_include_directories (
        fan_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        fan_lib
        PRIVATE
        hal_pwm_lib
        utility_observer_lib
        FreeRTOS::ARM_CM4F
        STM32::F4
)