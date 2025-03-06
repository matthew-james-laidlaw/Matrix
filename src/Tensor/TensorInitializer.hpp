#pragma once

#include <initializer_list>

#include <array>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>

#include <initializer_list>
#include <array>
#include <iostream>
#include <memory>
#include <numeric>
#include <vector>
#include <type_traits>

// Forward declaration
template<typename T, size_t N>
struct TensorType;

// Base case: N=1
template<typename T>
struct TensorType<T, 1> {
    using type = std::initializer_list<T>;
};

// Recursive case: N>1
template<typename T, size_t N>
struct TensorType {
    using type = std::initializer_list<typename TensorType<T, N - 1>::type>;
};

// Helper alias template
template<typename T, size_t N>
using TensorInitializer = typename TensorType<T, N>::type;

// The rest of your code remains largely the same
template <typename T, size_t Order>
auto InitializerListShape(TensorInitializer<T, Order> const& initializer) -> std::array<size_t, Order>
{
    std::array<size_t, Order> shape;
    shape[0] = initializer.size();
    if (shape[0] <= 0)
    {
        throw std::runtime_error("error: empty initializer list");
    }
    if constexpr (Order > 1)
    {
        auto first_sublist = *initializer.begin();
        auto subshape = InitializerListShape<T, Order - 1>(first_sublist);
        std::copy(subshape.begin(), subshape.end(), shape.begin() + 1);
        for (auto& sublist : initializer)
        {
            if (InitializerListShape<T, Order - 1>(sublist) != subshape)
            {
                throw std::runtime_error("error: inconsistent initializer shape");
            }
        }
    }
    return shape;
}

// Base case: When the initializer is a simple std::initializer_list<T>
template <typename T>
void FlattenImpl(TensorInitializer<T, 1> const& initializer, std::vector<T>& out)
{
    out.insert(out.end(), initializer.begin(), initializer.end());
}

// Recursive case: For nested initializer lists
template <typename T, size_t N>
void FlattenImpl(TensorInitializer<T, N> const& initializer, std::vector<T>& out)
{
    if constexpr (N == 1)
    {
		return FlattenImpl<T>(initializer, out);
    }
    else
    {
        for (const auto& sublist : initializer)
        {
            FlattenImpl<T, N - 1>(sublist, out);
        }
    }
}

// The flatten function: converts a nested initializer list into a contiguous storage array.
template <typename T, size_t Order>
auto InitializerListFlatten(TensorInitializer<T, Order> const& initializer) -> std::unique_ptr<T[]>
{
    auto shape = InitializerListShape<T, Order>(initializer);
    auto size = std::accumulate(shape.begin(), shape.end(), 1ull, std::multiplies<>());
    auto data = std::make_unique<T[]>(size);
    std::vector<T> tmp;
    tmp.reserve(size);
    FlattenImpl<T, Order>(initializer, tmp);
    std::copy(tmp.begin(), tmp.end(), data.get());
    return data;
}
