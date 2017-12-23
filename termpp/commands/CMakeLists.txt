add_library(commands
    STATIC
    arg.hpp
    command.hpp
    commands.hpp
    convert.hpp
)

target_link_libraries(commands
    INTERFACE
    utils
)

set_target_properties(commands PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(commands
    INTERFACE
    ${CMAKE_SOURCE_DIR}/thirdparty/callable_traits/include
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
# $<INSTALL_INTERFACE:${CMAKE_INSTALL_NAME_DIR}/category>
)