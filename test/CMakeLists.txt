find_package(GTest REQUIRED)

add_executable(Tests 
    shared_ptr_test.cpp
    vector_test.cpp
)
target_link_libraries(Tests
    gtest_main
    gtest
    cpp_examples
)
gtest_discover_tests(Tests)