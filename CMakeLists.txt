cmake_minimum_required(VERSION 3.13)
project(spis_100)
set(CMAKE_CXX_STANDARD 14)

find_package(Curses REQUIRED)
find_package(Threads)
include_directories(${CURSES_INCLUDE_DIR})


add_executable(spis_100 main.cpp consoleInput.cpp consoleOutput.cpp arrow.cpp file.cpp io.cpp io.h listInput.cpp listOutput.cpp node.cpp runtime.cpp spis.h system.cpp visualOutput.cpp)
target_link_libraries(spis_100 ${CURSES_LIBRARIES})
target_link_libraries (spis_100 ${CMAKE_THREAD_LIBS_INIT})