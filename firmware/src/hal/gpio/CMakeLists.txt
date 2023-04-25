message("Processing hal/gpio/CMakeList.txt")

add_library (
        hal_gpio_lib
        hal_gpio_types.h
        hal_gpio.h
        hal_gpio.c
)

target_include_directories (
        hal_gpio_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(HAL COMPONENTS
        LL_GPIO
        REQUIRED
        )

target_link_libraries(
        hal_gpio_lib
        PRIVATE
        CMSIS::STM32::F429xx
        HAL::STM32::F4::LL_GPIO
)