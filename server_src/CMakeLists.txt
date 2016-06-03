include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(server server.cpp)
target_link_libraries(server ${Boost_LIBRARIES})
install_programs(/server FILES server)

