cmake_minimum_required(VERSION 3.26)  # 设定最低版本的CMake要求，你可以根据你使用的CMake版本进行更改
project(WebServer)                   # 定义项目名称

set(CMAKE_CXX_STANDARD 17)

# 添加include目录到项目的包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)  # 添加src目录

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} Source)
