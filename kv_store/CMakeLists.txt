cmake_minimum_required(VERSION 3.26)  # 设定最低版本的CMake要求，你可以根据你使用的CMake版本进行更改
project(KVStore)                   # 定义项目名称

set(CMAKE_CXX_STANDARD 17)

# 添加include目录到项目的包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 引入FetchContent模块
include(FetchContent)

set(BENCHMARK_DOWNLOAD_DEPENDENCIES on)
# 声明要下载的Google Benchmark库
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.0  # 你可以换成你想要的版本
)

# 下载Google Benchmark
FetchContent_MakeAvailable(benchmark)

add_subdirectory(src)  # 添加src目录

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

# 添加基准测试的可执行文件
add_executable(store_benchmark tests/store_benchmark.cpp)

# 链接 Google Benchmark 库和项目代码到基准测试的可执行文件
target_link_libraries(store_benchmark benchmark Source)

target_link_libraries(${CMAKE_PROJECT_NAME} Source)
