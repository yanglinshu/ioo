cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME "IOO_BUILD")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

add_library(
  rust
  STATIC
  src/rust/rustdef.h
  src/rust/rustio.hpp
  src/rust/rustio.cc
)

target_include_directories(
  rust
  PUBLIC 
  src/rust/
)

add_library(
  ioo
  STATIC
  src/ioo/ioo.hpp
  src/ioo/utils.cc
  src/ioo/utils.hpp
)

target_include_directories(
  ioo
  PUBLIC 
  src/ioo/
  src/rust/
)

target_link_libraries(
  ioo
  rust
)

add_executable(
  ${PROJECT_NAME}
  src/main.cc
)

target_link_libraries(
  ${PROJECT_NAME}
  ioo
  rust
)

target_include_directories(
  ${PROJECT_NAME} 
  PUBLIC 
  src/
  src/ioo/
  src/rust/
)
