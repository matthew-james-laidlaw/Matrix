add_executable(unit_tests TestMatrix.cpp)
target_link_libraries(unit_tests gtest_main image)

gtest_discover_tests(unit_tests)
