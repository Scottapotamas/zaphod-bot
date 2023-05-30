message("Processing drivers/configuration")

add_library (
        configuration_lib
        configuration_types.h
        configuration.h
        configuration.c
)

target_include_directories (
        configuration_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        configuration_lib
        PRIVATE
#        hal_flash_lib
        FreeRTOS::ARM_CM4F
        STM32::F4
)