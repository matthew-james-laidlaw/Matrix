#pragma once

#include <Number.hpp>
#include <Expect.hpp>

#include <initializer_list>
#include <memory>
#include <tuple>

template <typename T>
using MatrixInitializer = std::initializer_list<std::initializer_list<T>>;

template <typename T>
std::tuple<usize, usize> Shape(MatrixInitializer<T> const& initializer)
{
    usize height = initializer.size();
    Expect(height > 0);

    usize width = initializer.begin()->size() > 0;
    Expect(width > 0);
    
    for (auto row : initializer)
    {
        Expect(row->size() > 0);
    }
}

template <typename T>
std::unique_ptr<T[]> Flatten(MatrixInitializer<T> const& initializer)
{
    ...
}
