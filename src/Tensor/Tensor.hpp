#pragma once

#include "Dispatcher.hpp"
#include "TensorInitializer.hpp"

#include <Expect.hpp>
#include <Number.hpp>

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

template <Number T, size_t N>
class Tensor
{
private:
    std::array<size_t, N> shape_;
    size_t size_;
    std::unique_ptr<T[]> data_;

public:
    Tensor() = delete;

    Tensor(std::array<size_t, N> const &shape)
        : shape_(shape), size_(SizeFromShape(shape_)), data_(new T[size_])
    {
    }

    Tensor(std::array<size_t, N> const &shape, T initializer)
        : Tensor(shape)
    {
        std::fill(data_.get(), data_.get() + size_, initializer);
    }

    Tensor(TensorInitializer<T, N> const& initializer)
    {
        std::tie(shape_, size_) = ShapeOfInitializer(initializer);
        data_ = Flatten(initializer);
    }

    Tensor(Tensor const &other)
        : Tensor(other.shape_)
    {
        std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
    }

    Tensor(Tensor &&other) noexcept
        : shape_(std::move(other.shape_)), size_(other.size_), data_(std::move(other.data_))
    {
        other.size_ = 0;
    }

    Tensor &operator=(Tensor const &other)
    {
        if (this == &other)
        {
            return *this;
        }

        shape_ = other.shape_;
        size_ = other.size_;
        data_.reset(new T[size_]);
        std::copy(other.data_.get(), other.data_.get() + size_, data_.get());

        return *this;
    }

    Tensor &operator=(Tensor &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        shape_ = std::move(other.shape_);
        size_ = other.size_;
        data_ = std::move(other.data_);

        other.size_ = 0;

        return *this;
    }

    ~Tensor() = default;

    auto Shape() const { return shape_; }
    auto Size() const { return size_; }
    auto Data() const { return data_.get(); }

    inline auto &operator()(std::array<size_t, N> indices)
    {
        return data_[Index(indices)];
    }

    inline auto operator()(std::array<size_t, N> indices) const
    {
        return data_[Index(indices)];
    }

private:
    static auto SizeFromShape(std::array<size_t, N> const &shape)
    {
        size_t accumulator = 1;
        for (auto dimension : shape)
        {
            accumulator *= dimension;
        }
        return accumulator;
    }

    inline auto Index(std::array<size_t, 1> const &indices) const
    {
        return indices[0];
    }

    inline auto Index(std::array<size_t, 2> const &indices) const
    {
        auto [y, x] = indices;
        auto [_, width] = shape_;
        return y * width + x;
    }

    inline auto Index(std::array<size_t, 3> const &indices) const
    {
        auto [z, y, x] = indices;
        auto [_, height, width] = shape_;
        return z * (height * width) +
               y * width +
               x;
    }
};

#include "Arithmetic.hpp"
