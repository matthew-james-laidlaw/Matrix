#include <gtest/gtest.h>

#include <Tensor.hpp>

#include <iostream>

TEST(A, A)
{
    std::cout << "Hello, World!" << std::endl;
    EXPECT_EQ(1, 1);
}

TEST(A, B)
{
    EXPECT_EQ(1, 1);
}
