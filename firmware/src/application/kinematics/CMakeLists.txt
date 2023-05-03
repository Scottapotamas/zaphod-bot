message("Processing application/kinematics")

add_library (
        kinematics_lib
        joint_types.h
        kinematics.c
        kinematics.h
        effector.h
        effector.c
)

target_include_directories (
        kinematics_lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        common
        PRIVATE
        ${FREERTOS_CONFIG_H}
)

# TODO: work out how to ensure the compile/link step uses hard-float for this unit without specifying the STM32::F4 package?
target_link_libraries(
        kinematics_lib
        PRIVATE
        FreeRTOS::ARM_CM4F
        STM32::F4
        )