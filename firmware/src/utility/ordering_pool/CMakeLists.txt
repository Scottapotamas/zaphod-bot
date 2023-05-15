message("Processing utility/ordering_pool")

add_library (
        utility_ordering_pool_lib
        request_handler.h
        request_handler.c
)

target_include_directories (
        utility_ordering_pool_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        utility_ordering_pool_lib
        PUBLIC
        utility_cartesian_lib
        utility_lighting_lib
        PRIVATE
        FreeRTOS::ARM_CM4F
        # TODO: work out how to compile/link with hard-float without specifying the STM32::F4 package?
        STM32::F4
)