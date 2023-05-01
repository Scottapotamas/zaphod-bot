message("Processing hal/adc")

add_library (
        hal_adc_lib
        hal_adc.h
        hal_adc.c
        hal_power.h
        hal_power.c
        hal_temperature.h
        hal_temperature.c
)

target_include_directories (
        hal_adc_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(HAL COMPONENTS
        STM32F4
        LL_DMA
        LL_ADC
        REQUIRED
        )

target_link_libraries(
        hal_adc_lib
        PRIVATE
        hal_gpio_lib
        CMSIS::STM32::F429xx
        HAL::STM32::F4::LL_DMA
        HAL::STM32::F4::LL_ADC
)