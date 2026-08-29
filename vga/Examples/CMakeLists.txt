add_executable(vga_demo
    main.cpp
)

target_include_directories(vga_demo PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(vga_demo
    vga_graphics
    pico_stdlib
)

pico_add_extra_outputs(vga_demo)
add_subdirectory(spaceinvader)
