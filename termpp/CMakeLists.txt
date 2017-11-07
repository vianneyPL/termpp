include_directories(${CMAKE_SOURCE_DIR}/thirdparty/brigand/include)

add_library(termpp
    STATIC
    term.cpp
    term.hpp
    cmd.hpp
    commands.hpp
    parse.hpp
    errors.hpp
    errors.cpp
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