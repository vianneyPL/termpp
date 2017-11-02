add_library(termpp
    STATIC
    term.cpp
    term.hpp
    control.hpp
    commands.hpp
    parse.hpp
)

add_subdirectory(utils)

target_link_libraries(termpp
    utils
)

set_target_properties(termpp PROPERTIES LINKER_LANGUAGE CXX)


target_include_directories(termpp INTERFACE
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
# $<INSTALL_INTERFACE:${CMAKE_INSTALL_NAME_DIR}/category>
)