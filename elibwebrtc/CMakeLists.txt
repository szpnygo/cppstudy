cmake_minimum_required(VERSION  3.26)
project(easywebrtc)
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

include(FetchContent)

# install external dependencies
include(cmake/libwebrtc.cmake)
include(cmake/ixwebsocket.cmake)
include(cmake/json.cmake)

set(SRC_FILES
    src/app.cpp
    src/peerconnection.cpp
    src/data_channel.cpp
    src/websockets.cpp
)

add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ${LIBWEBRTC})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ixwebsocket)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${libwebrtc_SOURCE_DIR}/include)

file(COPY ${LIBWEBRTC} DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(example)
add_subdirectory(test)
