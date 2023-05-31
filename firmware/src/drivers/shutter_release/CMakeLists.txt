message("Processing drivers/shutter_release")

add_library (
        shutter_release_lib
        shutter_release.h
        shutter_release.c
)

target_include_directories (
        shutter_release_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        shutter_release_lib
        PRIVATE
        hal_gpio_lib
        utility_observer_lib
        FreeRTOS::ARM_CM4F
        STM32::F4
)