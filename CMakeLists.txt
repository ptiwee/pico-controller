cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico-controller C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(pico-controller
    main.c
    usb_descriptors.c
)

target_include_directories(pico-controller PUBLIC
    ${CMAKE_CURRENT_LIST_DIR})
        
pico_enable_stdio_uart(pico-controller 1)

pico_add_extra_outputs(pico-controller)

target_link_libraries(pico-controller pico_stdlib tinyusb_device tinyusb_board)

set(CONTROLLER "NEOGEO" CACHE STRING "")

if(CONTROLLER STREQUAL "NEOGEO")
    target_compile_definitions(pico-controller PUBLIC NEOGEO)
elseif(CONTROLLER STREQUAL "ASTRO_CITY")
    target_compile_definitions(pico-controller PUBLIC ASTRO_CITY)
elseif(CONTROLLER STREQUAL "EGRET_II")
    target_compile_definitions(pico-controller PUBLIC EGRET_II)
elseif(CONTROLLER STREQUAL "SWITCH")
    target_compile_definitions(pico-controller PUBLIC SWITCH)
else()
    message(FATAL_ERROR "Wrong value for controller")
endif()
