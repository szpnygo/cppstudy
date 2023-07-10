cmake_minimum_required(VERSION 3.26)
project(HelloWorld)                   

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  filament_library
  GIT_REPOSITORY https://github.com/google/filament.git
  GIT_TAG        v1.40.0
  GIT_SHALLOW    ON
)

FetchContent_GetProperties(filament_library)
if(NOT filament_library_POPULATED)
  set(ENABLE_JAVA OFF CACHE INTERNAL "")
  set(FILAMENT_SUPPORTS_METAL OFF CACHE INTERNAL "")
  set(FILAMENT_SUPPORTS_VULKAN OFF CACHE INTERNAL "")
  set(FILAMENT_SKIP_SAMPLES ON CACHE INTERNAL "")
  FetchContent_Populate(filament_library)
  add_subdirectory(${filament_library_SOURCE_DIR} ${filament_library_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(${filament_library_SOURCE_DIR}/filament/include/filament)


message(STATUS "Source dir: ${filament_library_SOURCE_DIR}")
message(STATUS "Binary dir: ${filament_library_BINARY_DIR}")


add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${filament_library_SOURCE_DIR}/filament/include/filament)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "-framework CoreFoundation" "-framework CoreVideo" "-framework Metal" "-framework Foundation" "-framework AppKit")

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







