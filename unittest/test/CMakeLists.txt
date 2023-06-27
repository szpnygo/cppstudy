include(GoogleTest)

add_executable(calculator_test calculator_test.cpp ../src/calculator.cpp)
target_link_libraries(calculator_test gtest_main calculator_lib)
gtest_discover_tests(calculator_test)