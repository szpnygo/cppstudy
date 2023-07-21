cmake_minimum_required(VERSION  3.26)
project(easywebrtc)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# install external dependencies
include(cmake/libwebrtc.cmake)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SRC_FILES
    src/app.cpp
    src/peerconnection.cpp
)

add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBWEBRTC})


add_subdirectory(example)
add_subdirectory(test)
