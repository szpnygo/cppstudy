include(GoogleTest)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${hnsw_lib_SOURCE_DIR})

add_executable(hnsw_test hnsw_test.cpp)
target_link_libraries(hnsw_test gtest_main)
gtest_discover_tests(hnsw_test)