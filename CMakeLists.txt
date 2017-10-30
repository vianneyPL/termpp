cmake_minimum_required(VERSION 3.2)
project(termpp)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_COMPILER /usr/bin/clang-5.0)

add_subdirectory(termpp)
add_subdirectory(tests)
add_subdirectory(examples)