add_library(matrix INTERFACE)
target_include_directories(matrix INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(matrix INTERFACE foundation)
