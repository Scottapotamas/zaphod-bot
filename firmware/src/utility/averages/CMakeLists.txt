message("Processing utility/averages")

add_library (
        utility_averages_lib
        average_short.h
        average_short.c
)

target_include_directories (
        utility_averages_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
        utility_averages_lib
        PRIVATE
        STM32::F4
)