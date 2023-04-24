SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

# specify the cross compiler
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F429VETx_FLASH.ld)

#Uncomment for hardware floating point
SET(FPU_FLAGS "-mfloat-abi=hard -mfpu=fpv4-sp-d16")
add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1 )

#Uncomment for software floating point
#SET(FPU_FLAGS "-mfloat-abi=soft")

SET(COMMON_FLAGS
        "-mcpu=cortex-m4 ${FPU_FLAGS} -mthumb -mthumb-interwork -ffunction-sections -fdata-sections \
    -g -fno-common -fmessage-length=0 -specs=nosys.specs -specs=nano.specs")

SET(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -std=c++11")
SET(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-Wl,-gc-sections,--print-memory-usage -T ${LINKER_SCRIPT}")

PROJECT(delta-control C CXX ASM)
set(CMAKE_CXX_STANDARD 11)

# Automatically run the Git metadata embedding tool
IF (WIN32)
    add_custom_command(git_info BYPRODUCTS "${CMAKE_SOURCE_DIR}/src/app_state_machines/build-info.h"
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CreateBuildInfo.bat
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
ELSE()
    add_custom_target(git_info BYPRODUCTS "${CMAKE_SOURCE_DIR}/src/app_state_machines/build-info.h"
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/CreateBuildInfo.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
ENDIF()

add_definitions(-D__weak=__attribute__\(\(weak\)\) -D__packed=__attribute__\(\(__packed__\)\)  )

add_definitions(-DUSE_FULL_LL_DRIVER)

# External Clock Speed Defines
add_definitions(-DHSE_VALUE=8000000)
add_definitions(-DLSE_VALUE=32768)

# Internal Clock Speed Defines
add_definitions(-DHSI_VALUE=16000000)
add_definitions(-DLSI_VALUE=32000)
add_definitions(-DHSE_STARTUP_TIMEOUT=100)
add_definitions(-DLSE_STARTUP_TIMEOUT=5000)
add_definitions(-DEXTERNAL_CLOCK_VALUE=12288000)
add_definitions(-DVDD_VALUE=3300)
add_definitions(-DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1)

add_definitions(-DSTM32F429xx)

file(GLOB_RECURSE SOURCES "vendor/CMSIS/*.*"
                          "vendor/STM32F4xx_HAL_Driver/*.*"
                          "src/*.*"
                          "startup/*.*"
                          )

include_directories(src
                    src/app_state_machines
                    src/hal
                    src/drivers
                    src/drivers/demonstrations
                    src/utility
                    vendor/STM32F4xx_HAL_Driver/Inc
                    vendor/CMSIS/Device/ST/STM32F4xx/Include
                    vendor/CMSIS/Include
                    )

add_subdirectory(vendor/electricui-embedded)
add_subdirectory(vendor/electricui-interval-sender)

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${CMAKE_SOURCE_DIR}/src/app_state_machines/build-info.h ${LINKER_SCRIPT})
target_link_libraries(${PROJECT_NAME}.elf m electricui electricui-interval-sender)

set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map")

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")