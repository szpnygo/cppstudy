cmake_minimum_required(VERSION 3.26)
project(UnitTest)                   

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)

# 启用测试功能
include(CTest)

# 添加你的测试代码目录
add_subdirectory(test)

# include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

# add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

# target_link_libraries(${CMAKE_PROJECT_NAME} source)
