#pragma once

#include <algorithm>
#include <array>
#include <initializer_list>
#include <memory>
#include <numeric>
#include <tuple>
#include <vector>

// Define TensorInitializer correctly
template <typename T, size_t N>
struct TensorInitializerHelper
{
    using type = std::initializer_list<typename TensorInitializerHelper<T, N - 1>::type>;
};

// Base case: When N = 1, it's just std::initializer_list<T>
template <typename T>
struct TensorInitializerHelper<T, 1>
{
    using type = std::initializer_list<T>;
};

// Alias for easier use
template <typename T, size_t N>
using TensorInitializer = typename TensorInitializerHelper<T, N>::type;

// Compute the shape of an N-dimensional initializer list
template <typename T, size_t N>
std::array<size_t, N> ShapeOfInitializer(TensorInitializer<T, N> const& initializer)
{
    std::array<size_t, N> shape{};
    shape[0] = initializer.size();

    Expect(shape[0] > 0, "error: empty initializer list");

    if constexpr (N > 1)
    {
        auto first_sublist = *initializer.begin();
        auto subshape      = Shape<T, N - 1>(first_sublist);
        std::copy(subshape.begin(), subshape.end(), shape.begin() + 1);

        for (auto& sublist : initializer)
        {
            Expect(Shape<T, N - 1>(sublist) == subshape, "error: inconsistent initializer shape");
        }
    }

    return shape;
}

// Recursive function to flatten an N-dimensional initializer list
template <typename T, size_t N>
void FlattenImpl(TensorInitializer<T, N> const& initializer, std::vector<T>& out)
{
    for (const auto& sublist : initializer)
    {
        FlattenImpl<T, N - 1>(sublist, out);
    }
}

template <typename T>
void FlattenImpl(std::initializer_list<T> const& initializer, std::vector<T>& out)
{
    out.insert(out.end(), initializer.begin(), initializer.end());
}

// Flatten function to convert nested initializer list into contiguous storage
template <typename T, size_t N>
std::unique_ptr<T[]> Flatten(TensorInitializer<T, N> const& initializer)
{
    auto shape  = Shape<T, N>(initializer);
    size_t size = std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<>());

    auto data = std::make_unique<T[]>(size);
    std::vector<T> temp;
    temp.reserve(size);

    FlattenImpl<T, N>(initializer, temp);
    std::copy(temp.begin(), temp.end(), data.get());

    return data;
}
