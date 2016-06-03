cmake_minimum_required(VERSION 2.6)
project(Quake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -pedantic")
find_package(Boost 1.54 COMPONENTS system thread REQUIRED)

add_subdirectory(client_src)
add_subdirectory(game_include)
add_subdirectory(include)
add_subdirectory(server_src)

