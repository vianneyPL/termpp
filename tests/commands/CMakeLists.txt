include_directories(${CMAKE_SOURCE_DIR}/thirdparty/catch/single_include)

add_executable(test_termpp_commands
    main.cpp
    command.cpp
)

target_link_libraries(test_termpp_commands
    commands
)

add_test(NAME TestTermppCommands
    COMMAND test_termpp_commands
)
