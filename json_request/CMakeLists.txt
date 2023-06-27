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

FetchContent_MakeAvailable(cpr nlohmann_json)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/request.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE cpr::cpr nlohmann_json::nlohmann_json)

