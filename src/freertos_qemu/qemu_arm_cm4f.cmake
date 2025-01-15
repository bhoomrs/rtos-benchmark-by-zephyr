set(ARCH ARM_CM4F)

set(CONFIG_ARM ON)
set(CONFIG_CPU_CORTEX_M ON)
set(CONFIG_CORTEX_M_SYSTICK ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding -nostartfiles -nostdlib")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFREERTOS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__STARTUP_CLEAR_BSS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_CPU_CORTEX_M_HAS_VTOR")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPRINTF_ADVANCED_ENABLE=1")

set(CMAKE_TOOLCHAIN_FILE ${MCUX_SDK_PATH}/tools/cmake_toolchain_files/armgcc.cmake)

enable_language(ASM)

include_directories(src/freertos)
include_directories(${MCUX_SDK_PATH}/CMSIS/Core/Include)
include_directories(${MCUX_SDK_PATH}/components/serial_manager)
include_directories(${MCUX_SDK_PATH}/components/uart)

add_executable(app src/freertos/bench_porting_layer_freertos.c)

target_sources(app PRIVATE ${MCUX_SDK_PATH}/components/serial_manager/fsl_component_serial_manager.c)
target_sources(app PRIVATE ${MCUX_SDK_PATH}/components/serial_manager/fsl_component_serial_port_uart.c)
target_sources(app PRIVATE ${MCUX_SDK_PATH}/components/uart/fsl_adapter_uart.c)
target_sources(app PRIVATE ${MCUX_SDK_PATH}/devices/MK64F12/drivers/fsl_clock.c)
target_sources(app PRIVATE ${MCUX_SDK_PATH}/devices/MK64F12/drivers/fsl_smc.c)
target_sources(app PRIVATE ${MCUX_SDK_PATH}/devices/MK64F12/drivers/fsl_uart.c)
target_sources(app PRIVATE ${MCUX_SDK_PATH}/devices/MK64F12/utilities/debug_console/fsl_debug_console.c)
target_sources(app PRIVATE ${MCUX_SDK_PATH}/devices/MK64F12/utilities/fsl_sbrk.c)
target_sources(app PRIVATE ${MCUX_SDK_PATH}/devices/MK64F12/utilities/str/fsl_str.c)

target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/syscalls.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --specs=nano.specs")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --specs=nosys.specs")
set(LINKER_SCRIPT "/Users/bhoomrs/zephyrproject/rtos-benchmark-by-zephyr/src/freertos/qemu_arm_cm4f.ld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${LINKER_SCRIPT}")
target_link_libraries(app PRIVATE -Wl,--start-group)
target_link_libraries(app PRIVATE c)
target_link_libraries(app PRIVATE gcc)
target_link_libraries(app PRIVATE nosys)
target_link_libraries(app PRIVATE -Wl,--end-group)

set(EXEC_NAME freertos.elf)
set_target_properties(app PROPERTIES OUTPUT_NAME ${EXEC_NAME})

add_custom_target(flash USES_TERMINAL DEPENDS app COMMAND pyocd load --target k64f ${EXEC_NAME})
add_custom_target(debugserver USES_TERMINAL DEPENDS app COMMAND pyocd gdbserver --target k64f)
