message("Processing application/telemetry")

add_library (
        telemetry_lib
        user_interface_msgid.h
        user_interface_msgid.h
        user_interface.h
        user_interface.c
)

target_include_directories (
        telemetry_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        telemetry_lib
        PRIVATE
        hal_uart_lib
        hal_system_lib
        utility_observer_lib
        utility_cartesian_lib
        utility_lighting_lib
        electricui
        FreeRTOS::ARM_CM4F
        # TODO: work out how to compile/link with hard-float without specifying the STM32::F4 package?
        STM32::F4
)