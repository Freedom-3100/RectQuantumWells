cmake_minimum_required(VERSION 3.20)
project(RectQuantumWells VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(CoreLib)

option(BUILD_UI "Build Qt user interface" ON)
if(BUILD_UI)
    add_subdirectory(UI)
endif()

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()
