cmake_minimum_required(VERSION 3.5.1)

project(unknown-msg)

find_package(ignition-msgs1 REQUIRED)


add_executable(write-ign-msg
  write-ign-msg.cpp)

target_link_libraries(write-ign-msg PRIVATE ignition-msgs1::ignition-msgs1)


list(APPEND proto_args "--proto_path=${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND proto_args "--cpp_out=${CMAKE_CURRENT_BINARY_DIR}")
list(APPEND proto_args "${CMAKE_CURRENT_SOURCE_DIR}/unknown/unknown.proto")

add_custom_command(
  OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/unknown.pb.h"
    "${CMAKE_CURRENT_BINARY_DIR}/unknown.pb.cc"
  COMMAND
    protobuf::protoc
  ARGS
    ${proto_args})

add_executable(read-ign-msg
  read-ign-msg.cpp
  "${CMAKE_CURRENT_BINARY_DIR}/unknown/unknown.pb.cc")

target_link_libraries(read-ign-msg PRIVATE ignition-msgs1::ignition-msgs1)
target_include_directories(read-ign-msg PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
