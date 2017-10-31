add_library(termpp
    STATIC
    term.cpp
    term.hpp
    control.hpp
    commands.hpp
    parse.hpp
)

set_target_properties(termpp PROPERTIES LINKER_LANGUAGE CXX)


target_include_directories(termpp INTERFACE
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/termpp>
# $<INSTALL_INTERFACE:${CMAKE_INSTALL_NAME_DIR}/category>
)