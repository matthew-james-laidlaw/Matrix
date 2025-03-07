// clang-format off

#include <atomic>
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <utility>
#include <vector>
#include <stdexcept>

#include <Dispatcher.hpp>
#include <Expect.hpp>
#include <Number.hpp>
#include <PNG.hpp>
#include <PPM.hpp>
#include <Tensor.hpp>
#include <ThreadPool.hpp>

static const Tensor<int, 1> v1({ 2 },
{
    1, 2
});

static const Tensor<int, 2> m1({ 2, 2 },
{
    1, 2,
    3, 4,
});

static const Tensor<int, 2> m2({ 2, 3 },
{
    1, 2, 3,
    4, 5, 6,
});

static const Tensor<int, 3> t1({ 2, 2, 2 },
{
    1, 2,
    3, 4,

    5, 6,
    7, 8,
});

TEST(TensorTests, ElementwiseAddition)
{
    auto actual = m1 + m1;

    Tensor<int, 2> expected({ 2, 2 },
    {
        2, 4,
        6, 8,
    });

    EXPECT_EQ(expected, actual);
}

TEST(TensorTests, ElementwiseAdditionThrowsOnMismatchedDimensions)
{
    EXPECT_THROW(m1 + m2, std::runtime_error);
}

TEST(TensorTests, ElementwiseSubtraction_Successful)
{
    auto actual = m1 - m1;

    Tensor<int, 2> expected({ 2, 2 },
    {
        0, 0,
        0, 0,
    });

    EXPECT_EQ(expected, actual);
}

TEST(TensorTests, ElementwiseMultiplication_Successful)
{
    auto actual = m1 * m1;

    Tensor<int, 2> expected({ 2, 2 },
    {
        1,  4,
        9, 16,
    });

    EXPECT_EQ(expected, actual);
}

TEST(TensorTests, ElementwiseDivision_Successful)
{
    auto actual = m1 / m1;

    Tensor<int, 2> expected({ 2, 2 },
    {
        1, 1,
        1, 1,
    });

    EXPECT_EQ(expected, actual);
}

// ----- Tensor tests -----
TEST(TensorTest, ConstructionAndElementAccess)
{
    // Test construction with shape and initializer value.
    std::array<size_t, 2> shape = {2, 3};
    Tensor<int, 2> t(shape, 5);

    // Verify shape and that all elements are correctly initialized.
    EXPECT_EQ(t.Shape()[0], 2);
    EXPECT_EQ(t.Shape()[1], 3);
    for (size_t y = 0; y < shape[0]; ++y)
    {
        for (size_t x = 0; x < shape[1]; ++x)
        {
            EXPECT_EQ(t(y, x), 5);
        }
    }
}

TEST(TensorTest, ConstructionFromInitializerList)
{
    // Construct a 2D tensor from a nested initializer list.
    Tensor<int, 2> t({{1, 2}, {3, 4}});
    auto shape = t.Shape();
    EXPECT_EQ(shape[0], 2);
    EXPECT_EQ(shape[1], 2);
    EXPECT_EQ(t(0, 0), 1);
    EXPECT_EQ(t(0, 1), 2);
    EXPECT_EQ(t(1, 0), 3);
    EXPECT_EQ(t(1, 1), 4);
}

TEST(TensorTest, CopyAndMoveSemantics)
{
    Tensor<int, 2> original({2, 2}, 7);
    Tensor<int, 2> copy = original; // Copy constructor
    EXPECT_EQ(copy(0, 0), 7);

    // Modify original and ensure copy remains unchanged.
    original(0, 0) = 10;
    EXPECT_EQ(copy(0, 0), 7);

    // Test move semantics.
    Tensor<int, 2> moved = std::move(original);
    EXPECT_EQ(moved(0, 0), 10);
    // After moving, original's size is 0.
    EXPECT_EQ(original.Size(), 0);
}

TEST(TensorTest, DifferentNumericTypesAndInteractions)
{
    // Test with int tensor.
    Tensor<int, 2> t_int({2, 2}, 3);
    EXPECT_EQ(t_int(0, 0), 3);

    // Test with float tensor.
    Tensor<float, 2> t_float({2, 2}, 1.5f);
    EXPECT_FLOAT_EQ(t_float(1, 1), 1.5f);

    // Test interaction between different types via operator+.
    // The resulting tensor type should be the common type (float).
    auto result = t_int + t_float; // Uses operator+ from Arithmetic.hpp
    EXPECT_FLOAT_EQ(result(0, 0), 4.5f);
}

// ----- Arithmetic (Tensor operator overloads) tests -----
TEST(ArithmeticTest, TensorAddition)
{
    // Create two 2x2 tensors and add them.
    Tensor<int, 2> a({2, 2}, 1);
    Tensor<int, 2> b({2, 2}, 2);
    auto c = a + b;
    // Each element should be 1+2=3.
    EXPECT_EQ(c(0, 0), 3);
    EXPECT_EQ(c(1, 1), 3);
}

// ----- Dispatcher tests -----
TEST(DispatcherTest, Dispatch2dCallsCorrectly)
{
    std::vector<std::pair<size_t, size_t>> indices;
    // Use a 3x3 area so we expect 9 calls.
    Dispatch2d(3, 3, [&indices](size_t y, size_t x)
    {
        indices.emplace_back(y, x);
    });
    EXPECT_EQ(indices.size(), 9);
    // Optionally check that certain indices were covered.
    bool found00 = false, found11 = false, found22 = false;
    for (auto& idx : indices)
    {
        if (idx.first == 0 && idx.second == 0)
        {
            found00 = true;
        }
        if (idx.first == 1 && idx.second == 1)
        {
            found11 = true;
        }
        if (idx.first == 2 && idx.second == 2)
        {
            found22 = true;
        }
    }
    EXPECT_TRUE(found00);
    EXPECT_TRUE(found11);
    EXPECT_TRUE(found22);
}

// ----- ThreadPool tests -----
TEST(ThreadPoolTest, EnqueueAndExecute)
{
    std::atomic<int> counter(0);
    {
        // Create a thread pool with 4 threads.
        ThreadPool pool(4);
        for (int i = 0; i < 100; ++i)
        {
            pool.Enqueue([&counter]()
            {
                counter.fetch_add(1);
            });
        }
        // Wait for all tasks to finish.
        pool.Wait();
    }
    EXPECT_EQ(counter.load(), 100);
}

// clang-format on
