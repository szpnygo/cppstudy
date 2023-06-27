set(SOURCES 
    main.cpp
    calculator.cpp
    # 在此处添加更多源文件
)

add_library(calculator_lib STATIC ${SOURCES})
target_include_directories(calculator_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
