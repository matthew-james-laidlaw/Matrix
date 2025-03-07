#pragma once

#include <Number.hpp>
#include <Tensor.hpp>

#include <functional>

template <typename Operation, Number T1, Number T2, size_t N>
auto ElementwiseBinaryOperation(Tensor<T1, N> const& left, Tensor<T2, N> const& right)
{
    static_assert("matrix-matrix arithmetic is not defined for orders other than 2");
}

template <typename Operation, Number T1, Number T2, size_t N>
auto ElementwiseScalarOperation(Tensor<T1, N> const& left, T2 right)
{
    static_assert("scalar-matrix arithmetic is not defined for orders other than 2");
}

template <typename Operation, Number T1, Number T2, size_t N>
auto ElementwiseScalarOperation(T1 left, Tensor<T2, N> const& right)
{
    static_assert("scalar-matrix arithmetic is not defined for orders other than 2");
}

template <typename Operation, Number T1, Number T2>
auto ElementwiseBinaryOperation(Tensor<T1, 2> const& left, Tensor<T2, 2> const& right)
{
    Expect(left.Shape() == right.Shape());
    using ResultType = std::common_type_t<T1, T2>;
    Tensor<ResultType, 2> result(left.Shape(), 0);
    Operation operation;
    Dispatch2d(left.Shape()[0], left.Shape()[1], [&](size_t y, size_t x)
    {
        result(y, x) = operation(left(y, x), right(y, x));
    });
    return result;
}

template <typename Operation, Number T1, Number T2>
auto ElementwiseScalarOperation(Tensor<T1, 2> const& left, T2 right)
{
    using ResultType = std::common_type_t<T1, T2>;
    Tensor<ResultType, 2> result(left.Shape());
    Operation operation;
    Dispatch2d(left.Shape()[0], left.Shape()[1], [&](size_t y, size_t x)
    {
        result(y, x) = operation(left(y, x), right);
    });
    return result;
}

template <typename Operation, Number T1, Number T2>
auto ElementwiseScalarOperation(T1 left, Tensor<T2, 2> const& right)
{
    using ResultType = std::common_type_t<T1, T2>;
    Tensor<ResultType, 2> result(left.Shape());
    Operation operation;
    Dispatch2d(left.Shape()[0], left.Shape()[1], [&](size_t y, size_t x)
    {
        result(y, x) = operation(left, right(y, x));
    });
    return result;
}

template <Number T1, Number T2, size_t N>
auto operator+(Tensor<T1, N> const& left, Tensor<T2, N> const& right)
{
    return ElementwiseBinaryOperation<std::plus<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator-(Tensor<T1, N> const& left, Tensor<T2, N> const& right)
{
    return ElementwiseBinaryOperation<std::minus<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator*(Tensor<T1, N> const& left, Tensor<T2, N> const& right)
{
    return ElementwiseBinaryOperation<std::multiplies<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator/(Tensor<T1, N> const& left, Tensor<T2, N> const& right)
{
    return ElementwiseBinaryOperation<std::divides<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator+(Tensor<T1, N> const& left, T2 right)
{
    return ScalarBroadcastOperation<std::plus<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator+(T1 left, Tensor<T2, N> const& right)
{
    return ScalarBroadcastOperation<std::plus<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator-(Tensor<T1, N> const& left, T2 right)
{
    return ScalarBroadcastOperation<std::minus<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator-(T1 left, Tensor<T2, N> const& right)
{
    return ScalarBroadcastOperation<std::minus<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator*(Tensor<T1, N> const& left, T2 right)
{
    return ScalarBroadcastOperation<std::multiplies<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator*(T1 left, Tensor<T2, N> const& right)
{
    return ScalarBroadcastOperation<std::multiplies<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator/(Tensor<T1, N> const& left, T2 right)
{
    return ScalarBroadcastOperation<std::divides<>, T1, T2>(left, right);
}

template <Number T1, Number T2, size_t N>
auto operator/(T1 left, Tensor<T2, N> const& right)
{
    return ScalarBroadcastOperation<std::divides<>, T1, T2>(left, right);
}
