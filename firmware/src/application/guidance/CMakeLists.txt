message("Processing application/guidance")

add_library (
        guidance_lib
        cartesian_types.h
        movement_types.h
        cartesian_helpers.c
        cartesian_helpers.h
        cartesian_interpolator.c
        cartesian_interpolator.h
)

target_include_directories (
        guidance_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        common
)

# TODO: work out how to ensure the compile/link step uses hard-float for this unit without specifying the STM32::F4 package?
target_link_libraries(guidance_lib PRIVATE STM32::F4)