message("Processing hal/watchdog")

add_library (
        hal_watchdog_lib
        hal_watchdog.h
        hal_watchdog.c
)

target_include_directories (
        hal_watchdog_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE
        # Manually import the IWDG header file because the stm32-cmake tooling can't
        ${HAL_INCLUDE_DIRS}  #/stm32f4xx_ll_iwdg.h
)

# TODO: Resolve stm32-cmake tooling unable to handle header-only LL libs
#find_package(HAL COMPONENTS
#        LL_IWDG
#        REQUIRED
#        )

target_link_libraries(
        hal_watchdog_lib
        PRIVATE
#        HAL::STM32::F4::LL_IWDG
        CMSIS::STM32::F429xx
)