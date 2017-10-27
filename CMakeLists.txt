cmake_minimum_required(VERSION 3.2)
project(termpp)
enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(termpp)
add_subdirectory(tests)
add_subdirectory(examples)