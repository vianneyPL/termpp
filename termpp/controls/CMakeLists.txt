add_library(controls
    STATIC
    control.hpp
    controls.hpp
)

target_link_libraries(controls
    PUBLIC
    utils
)

set_target_properties(controls PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(controls
PUBLIC
    ${CMAKE_SOURCE_DIR}/thirdparty/callable_traits/include
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
# $<INSTALL_INTERFACE:${CMAKE_INSTALL_NAME_DIR}/category>
)