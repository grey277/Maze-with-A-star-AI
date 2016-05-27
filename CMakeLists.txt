cmake_minimum_required(VERSION 3.5)
project(quakeWithSockets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
find_package(Boost 1.58 COMPONENTS system thread REQUIRED)

set(SOURCE_FILES server.cpp server.h message.h session.h findShortestPath.h)
add_executable(server ${SOURCE_FILES})
add_executable(client client.cpp client.h message.h)
target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(client ${Boost_LIBRARIES})