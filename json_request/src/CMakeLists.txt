add_library(request_lib STATIC request.cpp)

target_include_directories(request_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
