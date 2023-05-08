message("Processing application/overwatch")

add_library (
        overwatch_lib
        overwatch.h
        overwatch.c
)

target_include_directories (
        overwatch_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)


target_link_libraries(
        overwatch_lib
        PRIVATE
        guidance_lib
        kinematics_lib
        utility_observer_lib
        telemetry_lib
        FreeRTOS::ARM_CM4F
        CMSIS::STM32::F429xx
        )