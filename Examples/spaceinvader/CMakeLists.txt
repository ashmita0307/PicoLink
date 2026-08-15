cmake_minimum_required(VERSION 3.13)

add_executable(space_invaders_game
    spaceinvade_mode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../vga_graphics.cpp
)

target_include_directories(space_invaders_game PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../..
)

target_link_libraries(space_invaders_game
    pico_stdlib
    hardware_pio
    hardware_dma
)

pico_enable_stdio_usb(space_invaders_game 1) 
pico_enable_stdio_uart(space_invaders_game 0) 
pico_add_extra_outputs(space_invaders_game)
