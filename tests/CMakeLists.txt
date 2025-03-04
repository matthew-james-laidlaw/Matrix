if (${PROJECT_NAME}_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

if (${PROJECT_NAME}_TESTS)
    add_subdirectory(unit)
endif()
