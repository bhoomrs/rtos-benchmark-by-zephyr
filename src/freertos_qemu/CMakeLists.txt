add_subdirectory(arch)
add_subdirectory(timer)

target_sources(app PRIVATE board.c)
target_sources(app PRIVATE clock_config.c)
target_sources(app PRIVATE pin_mux.c)

# target_link_options(app PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/qemu_arm_cm4f.ld)