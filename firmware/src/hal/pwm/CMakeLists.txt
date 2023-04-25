message("Processing hal/pwm/CMakeList.txt")

add_library (
        hal_pwm_lib
        hal_pwm.h
        hal_pwm.c
)

target_include_directories (
        hal_pwm_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(HAL COMPONENTS
        STM32F4
        LL_RCC
        LL_GPIO
        LL_TIM
        REQUIRED
        )

target_link_libraries(
        hal_pwm_lib
        PRIVATE
        hal_gpio_lib
        CMSIS::STM32::F429xx
        HAL::STM32::F4::LL_RCC
        HAL::STM32::F4::LL_GPIO
        HAL::STM32::F4::LL_TIM
)