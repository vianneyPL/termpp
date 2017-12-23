add_library(utils
    STATIC
    brigand_wrapper.hpp
    function_traits.hpp
    make_array.hpp
    split.hpp
)

set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(utils
    INTERFACE
    ${CMAKE_SOURCE_DIR}/thirdparty/brigand/include/standalone
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
# $<INSTALL_INTERFACE:${CMAKE_INSTALL_NAME_DIR}/category>
)