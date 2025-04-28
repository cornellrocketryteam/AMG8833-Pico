add_library(AMG8833-Pico INTERFACE)

target_sources(AMG8833-Pico INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/amg8833.cpp
)

target_link_libraries(AMG8833-Pico INTERFACE
    pico_stdlib
    hardware_i2c
)