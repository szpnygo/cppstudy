cmake_minimum_required(VERSION 3.26)  # 设定最低版本的CMake要求，你可以根据你使用的CMake版本进行更改
project(advanced_features) 

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)