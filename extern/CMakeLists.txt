add_subdirectory(argparse)

add_subdirectory(googletest)

if (${PROJECT_NAME}_ENABLE_BENCHMARKS)
    set(BENCHMARK_ENABLE_TESTING OFF)
    add_subdirectory(benchmark)
endif()
