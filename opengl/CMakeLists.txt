# 设置CMake最小版本要求
cmake_minimum_required(VERSION 3.26)

# 定义项目名称及使用的语言标准
project(OpenGL)
set(CMAKE_CXX_STANDARD 17)

# 包含CMake的FetchContent模块，FetchContent用于在构建期间下载和包含其他CMake项目
include(FetchContent)

# 声明glfw库的来源，包括其存储库的位置和要使用的版本
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)

# 声明glew库的来源，包括其存储库的位置和要使用的版本
FetchContent_Declare(
  glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
  GIT_TAG        origin/master
)

# # 如果glew还没有被下载和包含，那么下载并包含它
# FetchContent_GetProperties(glew)
# if(NOT glew_POPULATED)
#   FetchContent_Populate(glew)
#   add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
# endif()

# 如果glfw还没有被下载和包含，那么下载并包含它
FetchContent_MakeAvailable(glfw glew)

# 添加头文件的搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${glew_SOURCE_DIR}/include)
include_directories(${glfw_SOURCE_DIR}/include)

# 添加源文件的子目录
add_subdirectory(src)

# 添加可执行文件的生成规则
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

# 添加链接库规则
target_link_libraries(${CMAKE_PROJECT_NAME} glfw libglew_static)
