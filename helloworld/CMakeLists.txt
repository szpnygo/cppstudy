cmake_minimum_required(VERSION 3.26)  # 设定最低版本的CMake要求，你可以根据你使用的CMake版本进行更改
project(HelloWorld)                   # 定义项目名称

add_executable(HelloWorld main.cpp)   # 添加一个可执行文件，第一个参数是可执行文件的名称，第二个参数是用于生成它的源文件
