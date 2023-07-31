cmake_minimum_required(VERSION  3.26)
project(olivecdemo)
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


add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${stblib_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${olivec_SOURCE_DIR})

