message("Processing application/lighting")

add_library (
        lighting_lib
        led_interpolator.h
        led_interpolator.c
)

target_include_directories (
        lighting_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

# TODO: work out how to ensure the compile/link step uses hard-float for this unit without specifying the STM32::F4 package?
target_link_libraries(
        lighting_lib
        PRIVATE
        utility_lighting_lib
        utility_stopwatch_lib
        utility_observer_lib
        FreeRTOS::ARM_CM4F
        STM32::F4
        m
)