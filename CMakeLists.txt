cmake_minimum_required(VERSION 3.5)
project(quakeWithSockets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp server.h tcp_connection.h client.h)
add_executable(quakeWithSockets ${SOURCE_FILES})