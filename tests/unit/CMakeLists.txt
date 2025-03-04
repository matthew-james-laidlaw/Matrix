add_executable(unit_tests TestMatrix.cpp)
target_link_libraries(unit_tests gtest_main matrix)

gtest_discover_tests(unit_tests)
