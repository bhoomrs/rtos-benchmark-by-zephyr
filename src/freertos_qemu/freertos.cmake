# This file is included by "../../CMakeLists.txt".
# Therefore any specified relative paths are relative to "../../".

set(BOARD_CMAKE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/${BOARD}.cmake")
if (EXISTS ${BOARD_CMAKE_FILE})
    include(${BOARD_CMAKE_FILE})
else()
    message(FATAL_ERROR "Board configuration '${BOARD}.cmake' not found.")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFREERTOS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffreestanding")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPRINTF_ADVANCED_ENABLE=1")

include_directories(${FREERTOS_KERNEL_LOCAL}/include)
include_directories(${FREERTOS_KERNEL_LOCAL}/portable/GCC/${ARCH})
target_sources(app PRIVATE ${FREERTOS_KERNEL_LOCAL}/list.c)
target_sources(app PRIVATE ${FREERTOS_KERNEL_LOCAL}/portable/GCC/${ARCH}/port.c)
target_sources(app PRIVATE ${FREERTOS_KERNEL_LOCAL}/queue.c)
target_sources(app PRIVATE ${FREERTOS_KERNEL_LOCAL}/stream_buffer.c)
target_sources(app PRIVATE ${FREERTOS_KERNEL_LOCAL}/tasks.c)
target_sources(app PRIVATE ${FREERTOS_KERNEL_LOCAL}/timers.c)

set(EXEC_NAME freertos.elf)
set_target_properties(app PROPERTIES OUTPUT_NAME ${EXEC_NAME})
