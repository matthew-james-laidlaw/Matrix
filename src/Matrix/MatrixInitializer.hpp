#pragma once

#include <Number.hpp>
#include <Expect.hpp>

#include <initializer_list>
#include <memory>
#include <tuple>

template <typename T>
using MatrixInitializer = std::initializer_list<std::initializer_list<T>>;

template <typename T>
std::tuple<size_t, size_t> Shape(MatrixInitializer<T> const& initializer)
{
    size_t height = initializer.size();
    Expect(height > 0, "error: empty initializer list");

    size_t width = initializer.begin()->size();
    Expect(width > 0, "error: empty initializer list");
    
    for (auto& row : initializer)
    {
        Expect(row.size() == width, "error: inconsistent matrix initializer list shape");
    }

    return { height, width };
}

template <typename T>
std::unique_ptr<T[]> Flatten(MatrixInitializer<T> const& initializer)
{
    auto [height, width] = Shape(initializer);
    auto size = height * width;
    auto data = std::make_unique<T[]>(size);

    T* current = data.get();
    for (auto row : initializer)
    {
        std::copy(row.begin(), row.end(), current);
        current += width;
    }

    return data;
}
