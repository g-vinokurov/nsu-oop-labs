cmake_minimum_required(VERSION 3.0)
project("lab1")

set(CMAKE_CXX_STANDARD 14)
# enable_testing()
# add_subdirectory(googletest)
add_executable(app main.cpp Life.cpp Life.hpp utils.cpp utils.hpp LifeConfig.hpp LifeConfig.cpp LifeFileParser.hpp LifeFileParser.cpp LifeConstants.hpp)
# target_link_libraries(app gtest_main)
