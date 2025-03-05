#include <gtest/gtest.h>

#include <Tensor.hpp>

#include <iostream>

TEST(Tensor, ElementWiseAddition)
{
    auto m1 = Tensor<int, 2>({256, 256}, 1);
    auto m2 = Tensor<int, 2>({256, 256}, 2);
    auto m3 = m1 + m2;
}
