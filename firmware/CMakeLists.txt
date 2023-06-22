cmake_minimum_required(VERSION 3.16)
set(PROJ_NAME delta-control)
set(STM32_CMAKE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules/stm32-cmake)
set(CMAKE_TOOLCHAIN_FILE ${STM32_CMAKE_PATH}/cmake/stm32_gcc.cmake)
set(FREERTOS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules/freertos CACHE PATH "")

project(${PROJ_NAME} C ASM)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

stm32_fetch_cmsis(F4)
stm32_fetch_hal(F4)

find_package(CMSIS COMPONENTS STM32F4 REQUIRED)
find_package(HAL COMPONENTS STM32F4 LL_RCC REQUIRED)
find_package(FreeRTOS COMPONENTS ARM_CM4F REQUIRED)

add_definitions(-DUSE_FULL_LL_DRIVER)

# Clock speed definitions for custom hardware
add_definitions(-DHSE_VALUE=8000000)
add_definitions(-DLSE_VALUE=32768)

# Automatically run the Git metadata embedding tool
IF (WIN32)
    add_custom_command(git_info BYPRODUCTS "${CMAKE_SOURCE_DIR}/src/build_info.h"
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CreateBuildInfo.bat
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
ELSE()
    add_custom_target(git_info BYPRODUCTS "${CMAKE_SOURCE_DIR}/src/build_info.h"
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CreateBuildInfo.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
ENDIF()

add_executable(${PROJ_NAME})


set(FREERTOS_CONFIG_H ${CMAKE_SOURCE_DIR}/src/)
include_directories(src/common)

add_subdirectory(src/application)
add_subdirectory(src/drivers)
add_subdirectory(src/hal)
add_subdirectory(src/utility)

add_subdirectory(modules/electricui-embedded)

target_sources(
        ${PROJ_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/src/main.c
        ${CMAKE_SOURCE_DIR}/src/build_info.h
)

target_include_directories(
        ${PROJ_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${FREERTOS_CONFIG_H}
)

target_link_libraries(
        ${PROJ_NAME}
        PRIVATE
        app_startup_lib
        hal_system_lib
        hal_watchdog_lib
        FreeRTOS::Heap::4
        FreeRTOS::ARM_CM4F
        HAL::STM32::F4::LL_RCC
        CMSIS::STM32::F429xx
        STM32::NoSys
)

stm32_add_linker_script(${PROJ_NAME} PRIVATE F429VETx.ld)

stm32_generate_binary_file(${PROJ_NAME})
stm32_generate_hex_file(${PROJ_NAME})
stm32_print_size_of_target(${PROJ_NAME})
