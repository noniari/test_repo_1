cmake_minimum_required(VERSION 3.15)
project(Maze CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Maze
    src/main.cpp
    src/maze.cpp
    src/interface.cpp
    src/ai.cpp
)
