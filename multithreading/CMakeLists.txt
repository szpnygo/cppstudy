cmake_minimum_required(VERSION 3.26)  # 设定最低版本的CMake要求，你可以根据你使用的CMake版本进行更改
project(Multithreading)                   # 定义项目名称

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 添加include目录到项目的包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# # 添加src目录到项目的源文件目录
# set(SOURCES 
#     src/main.cpp
#     src/cat.cpp
#     src/dog.cpp
#     src/animal.cpp
#     # 在此处添加更多源文件
# )

add_subdirectory(src)  # 添加src目录

# add_executable(HelloWorld ${SOURCES})   # 添加一个可执行文件，第一个参数是可执行文件的名称，第二个参数是用于生成它的源文件
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} source)
