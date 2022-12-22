cmake_minimum_required(VERSION 3.0)
project("lab1")

set(CMAKE_CXX_STANDARD 14)
enable_testing()
add_subdirectory(googletest)
add_executable(app tuple.hpp csv.cpp csv.hpp csv_impl.hpp utils.hpp utils.cpp test.cpp)
target_link_libraries(app gtest_main)
