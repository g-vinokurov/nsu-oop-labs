cmake_minimum_required(VERSION 3.24)
project("lab1")

set(CMAKE_CXX_STANDARD 14)
# enable_testing()
# add_subdirectory(googletest)
add_executable(app main.cpp tuple_printer.hpp csv_parser.hpp)
# target_link_libraries(app gtest_main)
