cmake_minimum_required(VERSION 2.8.3)
project(AE_PB5S40A)
add_compile_options(-std=c++11)


include_directories(
  include
)


## Declare a C++ library
add_library(i2c_driver src/i2c_driver.cpp)
add_library(pb6s40a_control src/pb6s40a_control.cpp)

## Declare a C++ executable
add_executable(example_program src/example_program.cpp)

target_link_libraries(example_program i2c_driver pb6s40a_control)