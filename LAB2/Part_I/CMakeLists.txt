cmake_minimum_required(VERSION 3.16)
project(SpellChecker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(spellchecker
    src/main.cpp
    src/spellchecker.cpp
)

target_include_directories(spellchecker PRIVATE include)

configure_file(words.txt words.txt COPYONLY)