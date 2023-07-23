FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG        v11.4.4
)
set(USE_TLS ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(ixwebsocket)

