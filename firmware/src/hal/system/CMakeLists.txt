message("Processing hal/system/CMakeList.txt")

add_library (
        hal_system_lib
        hal_core.h
        hal_core.c
        hal_reset.h
        hal_reset.c
        hal_uuid.h
)

target_include_directories (
        hal_system_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(HAL COMPONENTS
        STM32F4
        LL_RCC
        LL_PWR
        LL_UTILS
        REQUIRED
        )

target_link_libraries(
        hal_system_lib
        PRIVATE
        hal_gpio_lib
        CMSIS::STM32::F429xx
        HAL::STM32::F4::LL_RCC
        HAL::STM32::F4::LL_PWR
        HAL::STM32::F4::LL_UTILS
)