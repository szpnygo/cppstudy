cmake_minimum_required(VERSION 3.26)
project(shared_memory)                   

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  stblib
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(stblib)

FetchContent_Declare(
  olivec
  GIT_REPOSITORY https://github.com/tsoding/olive.c.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(olivec)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${stblib_SOURCE_DIR})
include_directories(${olivec_SOURCE_DIR})


add_executable(writer src/writer.cpp)
add_executable(reader src/reader.cpp)
