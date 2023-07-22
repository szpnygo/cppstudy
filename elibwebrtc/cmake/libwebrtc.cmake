# download libwebrtc
FetchContent_Declare(
    libwebrtc
    GIT_REPOSITORY https://github.com/webrtc-sdk/libwebrtc.git
    GIT_TAG       d9ea233ddcfa8ac246b7031bc30d7ba0f3018902
)   

FetchContent_MakeAvailable(libwebrtc)

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

message(STATUS "libwebrtc: ${LIBWEBRTC}")