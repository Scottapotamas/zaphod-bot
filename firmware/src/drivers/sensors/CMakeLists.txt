message("Processing drivers/sensors")

add_library (
        sensors_lib
        sensors.h
        sensors.c
        observer.h
        observer.c
        subject.h
        subject.c
)

target_include_directories (
        sensors_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

target_link_libraries(
        sensors_lib
        PRIVATE
        hal_adc_lib
        hal_input_capture_lib
        utility_averages_lib
        FreeRTOS::ARM_CM4F
        # TODO: work out how to compile/link with hard-float without specifying the STM32::F4 package?
        STM32::F4
)