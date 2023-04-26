message("Processing hal/system/CMakeList.txt")

add_library (
        hal_uart_lib
        hal_uart.h
        hal_uart.c
)

target_include_directories (
        hal_uart_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        ${FREERTOS_CONFIG_H}
        common
)

find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

find_package(HAL COMPONENTS
        STM32F4
        LL_RCC
        LL_PWR
        LL_UTILS
        LL_GPIO
        LL_USART
        REQUIRED
)

target_link_libraries(
        hal_uart_lib
        PRIVATE
        hal_gpio_lib
        FreeRTOS::ARM_CM4F
        FreeRTOS::StreamBuffer
        CMSIS::STM32::F429xx
        HAL::STM32::F4::LL_RCC
        HAL::STM32::F4::LL_PWR
        HAL::STM32::F4::LL_UTILS
        HAL::STM32::F4::LL_GPIO
        HAL::STM32::F4::LL_USART
)