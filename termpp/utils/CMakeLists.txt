add_library(utils
    STATIC
    function_traits.hpp
    length.hpp
    make_array.hpp
    split.hpp
)

set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(utils INTERFACE
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
# $<INSTALL_INTERFACE:${CMAKE_INSTALL_NAME_DIR}/category>
)