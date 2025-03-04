#include <benchmark/benchmark.h>
#include "Matrix.hpp"

// // Benchmark for sequential matrix addition.
// static void BM_SequentialAddition(benchmark::State& state)
// {
//     // Matrix dimensions provided via benchmark arguments.
//     const usize rows = state.range(0);
//     const usize cols = state.range(1);

//     // Define a sequential matrix (using Naive arithmetic).
//     using MatrixSequential = Matrix<double, ThreadingPolicy::Sequential, ArithmeticPolicy::Naive>;
//     MatrixSequential A(rows, cols, 1.0);
//     MatrixSequential B(rows, cols, 2.0);

//     for (auto _ : state)
//     {
//         // Perform addition and prevent the result from being optimized away.
//         auto C = A + B;
//         benchmark::DoNotOptimize(C);
//     }
// }
// BENCHMARK(BM_SequentialAddition)->Args({1000, 1000});


// // Benchmark for threaded matrix addition.
// static void BM_ThreadedAddition(benchmark::State& state)
// {
//     const usize rows = state.range(0);
//     const usize cols = state.range(1);

//     // Define a threaded matrix (using Naive arithmetic).
//     using MatrixThreaded = Matrix<double, ThreadingPolicy::Threaded, ArithmeticPolicy::Naive>;
//     MatrixThreaded A(rows, cols, 1.0);
//     MatrixThreaded B(rows, cols, 2.0);

//     for (auto _ : state)
//     {
//         // Perform addition and prevent the result from being optimized away.
//         auto C = A + B;
//         benchmark::DoNotOptimize(C);
//     }
// }
// BENCHMARK(BM_ThreadedAddition)->Args({1000, 1000});
