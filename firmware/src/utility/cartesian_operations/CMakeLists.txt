message("Processing utility/cartesian_operations")

add_library (
        utility_cartesian_lib
        cartesian_helpers.c
        cartesian_helpers.h
        cartesian_interpolator.c
        cartesian_interpolator.h
        movement_types.h
)

target_include_directories (
        utility_cartesian_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        common
)

# TODO: work out how to ensure the compile/link step uses hard-float for this unit without specifying the STM32::F4 package?
target_link_libraries(
        utility_cartesian_lib
        PRIVATE
        STM32::F4
)