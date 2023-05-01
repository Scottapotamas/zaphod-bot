message("Processing hal/input_capture/CMakeList.txt")

add_library (
        hal_input_capture_lib
        hal_ic_hard.h
        hal_ic_hard.c
#        hal_ic_soft.h
#        hal_ic_soft.c
        hal_fan_tacho.h
        hal_fan_tacho.c
        hal_servo_hlfb.h
        hal_servo_hlfb.c
)

target_include_directories (
        hal_input_capture_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(HAL COMPONENTS
        LL_RCC
        LL_GPIO
        LL_TIM
        REQUIRED
        )

target_link_libraries(
        hal_input_capture_lib
        PRIVATE
        hal_gpio_lib
        CMSIS::STM32::F429xx
        HAL::STM32::F4::LL_RCC
        HAL::STM32::F4::LL_GPIO
        HAL::STM32::F4::LL_TIM
)