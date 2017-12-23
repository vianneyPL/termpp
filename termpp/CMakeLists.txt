add_library(termpp
    STATIC
    term.cpp
    term.hpp
    errors.hpp
    errors.cpp
    keys.hpp
    keys.cpp
)

add_subdirectory(utils)
add_subdirectory(commands)

target_include_directories(termpp
    INTERFACE
    ${CMAKE_SOURCE_DIR}/thirdparty/callable_traits/include
)

target_link_libraries(termpp
    utils
)

set_target_properties(termpp PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(termpp INTERFACE
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
# $<INSTALL_INTERFACE:${CMAKE_INSTALL_NAME_DIR}/category>
)