include_directories(${CMAKE_SOURCE_DIR}/thirdparty/catch/include)

add_executable(test_termpp
    main.cpp
    test_blank.cpp
)

target_link_libraries(test_termpp
    termpp
)

add_test(NAME TestTermpp
    COMMAND test_termpp
)
