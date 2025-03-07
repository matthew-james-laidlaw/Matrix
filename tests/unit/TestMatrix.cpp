#include <gtest/gtest.h>

#include <Tensor.hpp>

TEST(TensorTests, Addition)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto left     = Tensor<int, 2>({height, width}, 1);
    auto right    = Tensor<int, 2>({height, width}, 2);
    auto expected = Tensor<int, 2>({height, width}, 3);
    auto actual   = left + right;

    EXPECT_EQ(expected, actual);
}

TEST(TensorTests, Subtraction)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto left     = Tensor<int, 2>({height, width}, 3);
    auto right    = Tensor<int, 2>({height, width}, 2);
    auto expected = Tensor<int, 2>({height, width}, 1);
    auto actual   = left - right;

    EXPECT_EQ(expected, actual);
}

TEST(TensorTests, Multiplication)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto left     = Tensor<int, 2>({height, width}, 2);
    auto right    = Tensor<int, 2>({height, width}, 3);
    auto expected = Tensor<int, 2>({height, width}, 6);
    auto actual   = left * right;

    EXPECT_EQ(expected, actual);
}

TEST(TensorTests, Division)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto left     = Tensor<int, 2>({height, width}, 6);
    auto right    = Tensor<int, 2>({height, width}, 3);
    auto expected = Tensor<int, 2>({height, width}, 2);
    auto actual   = left / right;

    EXPECT_EQ(expected, actual);
}

TEST(TensorTests, AdditionThrowsOnMismatchedDimensions)
{
    auto left  = Tensor<int, 2>({2, 3});
    auto right = Tensor<int, 2>({3, 2});
    EXPECT_THROW(left + right, std::runtime_error);
}

TEST(TensorTests, SubtractionThrowsOnMismatchedDimensions)
{
    auto left  = Tensor<int, 2>({2, 3});
    auto right = Tensor<int, 2>({3, 2});
    EXPECT_THROW(left - right, std::runtime_error);
}

TEST(TensorTests, MultiplicationThrowsOnMismatchedDimensions)
{
    auto left  = Tensor<int, 2>({2, 3});
    auto right = Tensor<int, 2>({3, 2});
    EXPECT_THROW(left * right, std::runtime_error);
}

TEST(TensorTests, DivisionThrowsOnMismatchedDimensions)
{
    auto left  = Tensor<int, 2>({2, 3});
    auto right = Tensor<int, 2>({3, 2});
    EXPECT_THROW(left / right, std::runtime_error);
}
