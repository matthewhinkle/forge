cmake_minimum_required(VERSION 3.2)
project(Forge)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I${CMAKE_ROOT}/src")

add_definitions(-DFORGE_WINDOW_X)

set(SOURCE_FILES src/main.cpp)
add_executable(Forge ${SOURCE_FILES} src/window.cpp src/common/logger.cpp src/window/x-window.cpp src/window/window-incl.h)