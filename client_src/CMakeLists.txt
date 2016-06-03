include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(client client.cpp)
target_link_libraries(client ${Boost_LIBRARIES} ${CURSES_LIBRARIES})
install_programs(/client FILES client)