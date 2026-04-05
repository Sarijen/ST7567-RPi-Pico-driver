cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(ST7567 C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(ST7567
  src/ST7567.cpp
  src/font_5x8.cpp
  src/font_10x16.cpp

)

target_include_directories(ST7567 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(
  ST7567
  pico_stdlib
  hardware_spi
)

add_executable(ST7567_example
  example_code/testing.cpp
)

target_link_libraries(ST7567_example PRIVATE ST7567)

target_compile_options(ST7567_example PRIVATE
  -Wall
  -Wundef
)

pico_add_extra_outputs(ST7567_example)
