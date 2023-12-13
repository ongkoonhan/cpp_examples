find_package(GTest REQUIRED)

set(gtest_libs
    gtest_main
    gtest
    gmock
)

# examples tests
add_executable(Tests 
    shared_ptr_test.cpp
    vector_test.cpp
)
target_link_libraries(Tests
    cpp_examples
    ${gtest_libs}
)

# memory tests
add_executable(TestsMemory memory_test.cpp)
target_link_libraries(TestsMemory
    cpp_examples
    ${gtest_libs}
)
# set_source_files_properties(memory_test.cpp PROPERTIES COMPILE_FLAGS -fsanitize=address)
target_compile_options(TestsMemory PRIVATE -fsanitize=address)
target_link_options(TestsMemory PRIVATE -fsanitize=address)

# gtest
gtest_discover_tests(Tests)
gtest_discover_tests(TestsMemory)