cmake_minimum_required(VERSION 3.0)

project("lab0")

enable_testing()
add_subdirectory(googletest)
add_executable(testapp test.cpp bigint/bigint.cpp bigint/bigint.hpp)
target_link_libraries(testapp gtest_main)
