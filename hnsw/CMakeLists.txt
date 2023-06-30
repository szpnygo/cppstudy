cmake_minimum_required(VERSION 3.26)
project(hnsw)                   

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

include(FetchContent)

FetchContent_Declare(
  hnsw_lib
  GIT_REPOSITORY https://github.com/nmslib/hnswlib.git
  GIT_TAG        v0.7.0
)

FetchContent_MakeAvailable(hnsw_lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(${hnsw_lib_SOURCE_DIR})

add_subdirectory(src)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE source)

