# add google test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)

include(CTest)
include(GoogleTest)

function(add_test TEST_NAME)
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} gtest_main easywebrtc)
    gtest_discover_tests(${TEST_NAME})
endfunction()

file(COPY ${LIBWEBRTC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(app_test)