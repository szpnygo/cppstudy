cmake_minimum_required(VERSION 3.26)  # 设定最低版本的CMake要求，你可以根据你使用的CMake版本进行更改
project(json_request) 

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

include(FetchContent)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/whoshuu/cpr.git
  GIT_TAG        1.10.4
)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.2
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)

FetchContent_MakeAvailable(cpr nlohmann_json googletest)

# 启用测试功能
include(CTest)

# 添加你的测试代码目录
add_subdirectory(test)

add_subdirectory(src)

target_link_libraries(request_lib PRIVATE cpr::cpr nlohmann_json::nlohmann_json)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE request_lib)

