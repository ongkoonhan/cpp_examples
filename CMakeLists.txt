cmake_minimum_required (VERSION 3.22.1)
project (cpp_examples)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()