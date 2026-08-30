cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pacman C CXX ASM)

pico_sdk_init()

add_executable(pacman
    Examples/pacman_mode.cpp
    vga_graphics.cpp
)

pico_enable_stdio_usb(pacman 1)
pico_enable_stdio_uart(pacman 0)

pico_generate_pio_header(pacman
    ${CMAKE_CURRENT_LIST_DIR}/rgb.pio
)

target_include_directories(pacman PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(pacman
    pico_stdlib
    hardware_pio
    hardware_dma
)

pico_add_extra_outputs(pacman)
