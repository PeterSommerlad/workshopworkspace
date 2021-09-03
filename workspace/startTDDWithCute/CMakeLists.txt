cmake_minimum_required(VERSION 3.20)

project(cutetest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(cutetest src/Test.cpp)



target_include_directories(cutetest PUBLIC cute/ src/)




