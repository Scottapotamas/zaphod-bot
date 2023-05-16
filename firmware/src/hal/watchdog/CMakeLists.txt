message("Processing hal/watchdog/CMakeList.txt")

add_library (
        hal_watchdog_lib
        hal_watchdog.h
        hal_watchdog.c
)

target_include_directories (
        hal_watchdog_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(HAL COMPONENTS
        LL_IWDG   # TODO RESOLVE TOOLING ISSUE FINDING HEADER-ONLY
        REQUIRED
        )

target_link_libraries(
        hal_watchdog_lib
        PRIVATE
        CMSIS::STM32::F429xx
        HAL::STM32::F4::LL_IWDG
)