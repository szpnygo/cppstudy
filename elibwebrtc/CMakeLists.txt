cmake_minimum_required(VERSION  3.26)
project(easywebrtc)

set(CMAKE_CXX_STANDARD 17)

# download libwebrtc
include(FetchContent)
FetchContent_Declare(
    libwebrtc
    GIT_REPOSITORY https://github.com/webrtc-sdk/libwebrtc.git
    GIT_TAG       d9ea233ddcfa8ac246b7031bc30d7ba0f3018902
)   

FetchContent_MakeAvailable(libwebrtc)
include_directories(${libwebrtc_SOURCE_DIR}/include)

# enable desktop device
add_definitions(-DRTC_DESKTOP_DEVICE)

# find libwebrtc
set(LIBWEBRTC_DIR "")
if(WIN32)
    set(LIBWEBRTC_DIR "win_x64")
elseif(APPLE)
    set(LIBWEBRTC_DIR "mac_x64")
endif()

if(LIBWEBRTC_DIR)
    find_library(
        LIBWEBRTC
        NAMES webrtc
        HINTS ${CMAKE_SOURCE_DIR}/libwebrtc/${LIBWEBRTC_DIR}
    )
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SRC_FILES
    src/app.cpp
)

# create library
add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC_FILES})

# link libwebrtc
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${LIBWEBRTC})

# define example function
function(add_example EXAMPLE_NAME)
    set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME}_example_${EXAMPLE_NAME})
    add_executable(${EXECUTABLE_NAME} example/${EXAMPLE_NAME}.cpp)
    target_link_libraries(${EXECUTABLE_NAME}  PRIVATE ${CMAKE_PROJECT_NAME})

    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        ${LIBWEBRTC} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
endfunction()

add_example(hello)