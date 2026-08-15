cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(VGA_Graphics C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# Include directories
include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
)

# VGA library
add_library(vga_graphics
    vga_graphics.cpp
)

target_link_libraries(vga_graphics
    pico_stdlib
    hardware_pio
    hardware_dma
    hardware_irq
    hardware_clocks
)

# Build the example
add_subdirectory(Examples)