message("Processing drivers/buzzer")

add_library (
        buzzer_lib
        buzzer.h
        buzzer.c
)

target_include_directories (
        buzzer_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        buzzer_lib
        PRIVATE
        hal_pwm_lib
        FreeRTOS::ARM_CM4F
        STM32::F4
)