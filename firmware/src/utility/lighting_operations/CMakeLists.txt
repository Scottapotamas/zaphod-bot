message("Processing utility/lighting_operations")

add_library (
        utility_lighting_lib
        lighting_types.h
        led_helpers.h
        led_helpers.c
        led_interpolators.h
        led_interpolators.c
        noise.h
        noise.c
)

target_include_directories (
        utility_lighting_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        common
)

target_link_libraries(
        utility_lighting_lib
        PRIVATE
        STM32::F4
)