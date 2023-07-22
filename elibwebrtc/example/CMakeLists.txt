# define example function
function(add_example EXAMPLE_NAME)
    set(EXECUTABLE_NAME ${CMAKE_PROJECT_NAME}_example_${EXAMPLE_NAME})
    add_executable(${EXECUTABLE_NAME} ${EXAMPLE_NAME}.cpp)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${CMAKE_PROJECT_NAME})
endfunction()

file(COPY ${LIBWEBRTC} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


add_example(hello)