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

        # TODO: work out how to compile/link with hard-float without specifying the STM32::F4 package?
        STM32::F4
)