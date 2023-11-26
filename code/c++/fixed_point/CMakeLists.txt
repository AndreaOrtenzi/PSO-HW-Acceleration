cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(PSO LANGUAGES CXX VERSION 0.1)
add_executable(${PROJECT_NAME} main.cpp PSO.cpp)
