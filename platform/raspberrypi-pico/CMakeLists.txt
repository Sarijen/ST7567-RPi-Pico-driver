cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(ST7567_example C CXX)

pico_sdk_init()

add_subdirectory(../../ ST7567)

add_executable(ST7567_example
  main.cpp
)

target_compile_features(ST7567_example PRIVATE
  cxx_std_20
)

target_link_libraries(
  ST7567_example
  PRIVATE

  pico_stdlib
  hardware_spi

  ST7567
)

target_compile_options(ST7567_example PRIVATE
  -Wall
  -Wundef
)

pico_add_extra_outputs(ST7567_example)
