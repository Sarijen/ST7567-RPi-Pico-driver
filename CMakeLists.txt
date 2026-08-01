cmake_minimum_required(VERSION 3.12)

project(ST7567 C CXX)

add_library(ST7567
  STATIC
  src/font_5x8.cpp
  src/font_10x16.cpp
)

target_include_directories(ST7567
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_compile_features(ST7567 PRIVATE
  c_std_11
  cxx_std_20
)
