message("Processing application/fan/CMakeList.txt")

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
        hal_input_capture_lib
        FreeRTOS::ARM_CM4F
        # TODO: work out how to compile/link with hard-float without specifying the STM32::F4 package?
        STM32::F4
)