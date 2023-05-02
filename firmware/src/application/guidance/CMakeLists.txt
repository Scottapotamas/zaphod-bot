message("Processing application/guidance")

add_library (
        guidance_lib
        cartesian_types.h
        movement_types.h
        cartesian_helpers.c
        cartesian_helpers.h
        cartesian_interpolator.c
        cartesian_interpolator.h
        request_handler.h
        request_handler.c
)

target_include_directories (
        guidance_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

# TODO: work out how to ensure the compile/link step uses hard-float for this unit without specifying the STM32::F4 package?
target_link_libraries(
        guidance_lib
        PRIVATE
        FreeRTOS::ARM_CM4F
        STM32::F4
)