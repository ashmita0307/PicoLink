cmake_minimum_required(VERSION 3.13)

add_executable(dev_hid_cdc)

target_sources(dev_hid_cdc PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
)

target_include_directories(dev_hid_cdc PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(dev_hid_cdc PUBLIC
        pico_stdlib
        pico_unique_id
        tinyusb_device
        tinyusb_board
)

pico_add_extra_outputs(dev_hid_cdc)

example_auto_set_url(dev_hid_cdc)