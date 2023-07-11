# 设置CMake最小版本要求
cmake_minimum_required(VERSION 3.26)

# 定义项目名称及使用的语言标准
project(Filament)
set(CMAKE_CXX_STANDARD 17)

# 包含CMake的FetchContent模块，FetchContent用于在构建期间下载和包含其他CMake项目
include(FetchContent)

# 声明filament库的来源
FetchContent_Declare(
  filament_library
  GIT_REPOSITORY https://github.com/google/filament.git
  GIT_TAG        v1.40.0
  GIT_SHALLOW    ON
)

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

FetchContent_GetProperties(filament_library)
if(NOT filament_library_POPULATED)
  set(ENABLE_JAVA OFF CACHE INTERNAL "")
  set(FILAMENT_SUPPORTS_METAL OFF CACHE INTERNAL "")
  set(FILAMENT_SUPPORTS_VULKAN OFF CACHE INTERNAL "")
  set(FILAMENT_SUPPORTS_OPENGL ON CACHE INTERNAL "")
  set(FILAMENT_SKIP_SAMPLES OFF CACHE INTERNAL "")
  FetchContent_Populate(filament_library)
  add_subdirectory(${filament_library_SOURCE_DIR} ${filament_library_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_MakeAvailable(glfw glew)


include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${glew_SOURCE_DIR}/include)
include_directories(${glfw_SOURCE_DIR}/include)
include_directories(${filament_library_SOURCE_DIR}/filament/include/filament)


add_executable(${CMAKE_PROJECT_NAME} src/main.mm)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "-framework CoreFoundation" "-framework CoreVideo" "-framework Metal" "-framework Foundation" "-framework AppKit")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw libglew_static)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  math
  filameshio
  filament
  filamat_lite
  filamat
  filaflat
  filabridge
  geometry
  backend
  bluegl
  ibl
  image
  meshoptimizer
  smol-v
  utils)







