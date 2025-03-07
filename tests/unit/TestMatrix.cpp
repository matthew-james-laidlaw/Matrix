// clang-format off

#include <gtest/gtest.h>

#include <Tensor.hpp>

#include <array>

TEST(MatrixTests, DefaultConstructor)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto tensor = Tensor<int, 2>({height, width});

    EXPECT_EQ(tensor.Shape(), (std::array<size_t, 2>{height, width}));
    EXPECT_EQ(tensor.Size(), 1024 * 1024);
}

TEST(MatrixTests, FillConstructor)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto tensor = Tensor<int, 2>({height, width}, 1);

    EXPECT_EQ(tensor.Shape(), (std::array<size_t, 2>{height, width}));
    EXPECT_EQ(tensor.Size(), height * width);

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            EXPECT_EQ(tensor(y, x), 1);
        }
    }
}

TEST(MatrixTests, InitializerListConstructor)
{
    size_t height = 2;
    size_t width = 2;
    auto tensor = Tensor<int, 2> {
        { height, width },
        {
            1, 2,
            3, 4,
        }
    };

    EXPECT_EQ(tensor.Shape(), (std::array<size_t, 2>{height, width}));
    EXPECT_EQ(tensor.Size(), height * width);
    
    EXPECT_EQ(tensor(0, 0), 1);
    EXPECT_EQ(tensor(0, 1), 2);
    EXPECT_EQ(tensor(1, 0), 3);
    EXPECT_EQ(tensor(1, 1), 4);
}

TEST(MatrixTests, InitializerListConstructorThrowsOnMismatchedDimensions)
{
    size_t height = 3;
    size_t width = 3;
    EXPECT_THROW(([&]()
    {
        auto tensor = Tensor<int, 2> {
            { height, width },
            {
                1, 2,
                3, 4,
            }
        };
    })(),
    std::runtime_error);
}

TEST(MatrixTests, CopyConstructor)
{
}

TEST(MatrixTests, MoveConstructor)
{
}

TEST(MatrixTests, CopyAssignment)
{
}

TEST(MatrixTests, MoveAssignment)
{
}

TEST(MatrixTests, Addition)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto left     = Tensor<int, 2>({height, width}, 1);
    auto right    = Tensor<int, 2>({height, width}, 2);
    auto expected = Tensor<int, 2>({height, width}, 3);
    auto actual   = left + right;

    EXPECT_EQ(expected, actual);
}

TEST(MatrixTests, Subtraction)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto left     = Tensor<int, 2>({height, width}, 3);
    auto right    = Tensor<int, 2>({height, width}, 2);
    auto expected = Tensor<int, 2>({height, width}, 1);
    auto actual   = left - right;

    EXPECT_EQ(expected, actual);
}

TEST(MatrixTests, Multiplication)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto left     = Tensor<int, 2>({height, width}, 2);
    auto right    = Tensor<int, 2>({height, width}, 3);
    auto expected = Tensor<int, 2>({height, width}, 6);
    auto actual   = left * right;

    EXPECT_EQ(expected, actual);
}

TEST(MatrixTests, Division)
{
    size_t height = 1024;
    size_t width  = 1024;

    auto left     = Tensor<int, 2>({height, width}, 6);
    auto right    = Tensor<int, 2>({height, width}, 3);
    auto expected = Tensor<int, 2>({height, width}, 2);
    auto actual   = left / right;

    EXPECT_EQ(expected, actual);
}

TEST(MatrixTests, AdditionThrowsOnMismatchedDimensions)
{
    auto left  = Tensor<int, 2>({2, 3});
    auto right = Tensor<int, 2>({3, 2});
    EXPECT_THROW(left + right, std::runtime_error);
}

TEST(MatrixTests, SubtractionThrowsOnMismatchedDimensions)
{
    auto left  = Tensor<int, 2>({2, 3});
    auto right = Tensor<int, 2>({3, 2});
    EXPECT_THROW(left - right, std::runtime_error);
}

TEST(MatrixTests, MultiplicationThrowsOnMismatchedDimensions)
{
    auto left  = Tensor<int, 2>({2, 3});
    auto right = Tensor<int, 2>({3, 2});
    EXPECT_THROW(left * right, std::runtime_error);
}

TEST(MatrixTests, DivisionThrowsOnMismatchedDimensions)
{
    auto left  = Tensor<int, 2>({2, 3});
    auto right = Tensor<int, 2>({3, 2});
    EXPECT_THROW(left / right, std::runtime_error);
}

// clang-format on
