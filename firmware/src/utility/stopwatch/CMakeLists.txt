message("Processing utility/stopwatch")

add_library (
        utility_stopwatch_lib
        stopwatch.h
        stopwatch.c
)

target_include_directories (
        utility_stopwatch_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
)

target_link_libraries(
        utility_stopwatch_lib
        PRIVATE
        FreeRTOS::ARM_CM4F
        STM32::F4
)