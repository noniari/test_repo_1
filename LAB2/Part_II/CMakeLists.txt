cmake_minimum_required(VERSION 3.16)
project(GreedyAlgorithms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

foreach(task_number RANGE 1 6)
    add_subdirectory(Task${task_number})
endforeach()