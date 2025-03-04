add_subdirectory(argparse)

if (${PROJECT_NAME}_TESTS)
    add_subdirectory(googletest)
endif()

if (${PROJECT_NAME}_BENCHMARKS)
    add_subdirectory(benchmark)
endif()
