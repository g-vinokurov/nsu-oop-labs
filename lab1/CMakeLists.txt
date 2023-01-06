cmake_minimum_required(VERSION 3.0)
project("lab1")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(app main.cpp Life.cpp Life.hpp utils.cpp utils.hpp LifeConfig.hpp LifeConfig.cpp LifeFileParser.hpp LifeFileParser.cpp LifeConstants.hpp App.hpp App.cpp Qt5GUI.hpp Qt5GUI.cpp Presenter.hpp Presenter.cpp)
target_link_libraries(app Qt5::Core Qt5::Gui Qt5::Widgets)

