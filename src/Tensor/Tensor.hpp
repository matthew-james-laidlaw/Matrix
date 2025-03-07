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

template <size_t N>
concept ValidOrder = (N == 1 || N == 2 || N == 3);

template <size_t Order, typename... Indices>
concept ValidIndices = (sizeof...(Indices) == Order && (std::is_convertible_v<Indices, size_t> && ...));

template <Number T, size_t Order>
    requires ValidOrder<Order>
class Tensor
{
private:

    std::array<size_t, Order> shape_;
    size_t size_;
    std::unique_ptr<T[]> data_;

public:

    Tensor() = delete;

    Tensor(std::array<size_t, Order> const& shape)
        : shape_(shape)
        , size_(Size(shape_))
        , data_(new T[size_])
    {}

    Tensor(std::array<size_t, Order> const& shape, T initializer)
        : Tensor(shape)
    {
        std::fill(data_.get(), data_.get() + size_, initializer);
    }

    Tensor(TensorInitializer<T, Order> const& initializer)
        : shape_(InitializerShape<T, Order>(initializer))
        , size_(Size(shape_))
        , data_(InitializerFlatten<T, Order>(initializer))
    {}

    Tensor(Tensor const& other)
        : Tensor(other.shape_)
    {
        std::copy(other.data_.get(), other.data_.get() + size_, data_.get());
    }

    Tensor(Tensor&& other) noexcept
        : shape_(std::move(other.shape_))
        , size_(other.size_)
        , data_(std::move(other.data_))
    {
        other.size_ = 0;
    }

    Tensor& operator=(Tensor const& other)
    {
        if (this == &other)
        {
            return *this;
        }

        shape_ = other.shape_;
        size_  = other.size_;
        data_.reset(new T[size_]);
        std::copy(other.data_.get(), other.data_.get() + size_, data_.get());

        return *this;
    }

    Tensor& operator=(Tensor&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        shape_ = std::move(other.shape_);
        size_  = other.size_;
        data_  = std::move(other.data_);

        other.size_ = 0;

        return *this;
    }

    ~Tensor() = default;

    auto Shape() const
    {
        return shape_;
    }

    auto Size() const
    {
        return size_;
    }

    auto Data() const
    {
        return data_.get();
    }

    template <typename... Indices>
        requires ValidIndices<Order, Indices...>
    inline auto& operator()(Indices... indices)
    {
        return data_[LinearIndex(indices...)];
    }

    template <typename... Indices>
        requires ValidIndices<Order, Indices...>
    inline auto operator()(Indices... indices) const
    {
        return data_[LinearIndex(indices...)];
    }

private:

    static auto Size(std::array<size_t, Order> const& shape)
    {
        return std::accumulate(shape.begin(), shape.end(), 1ull, std::multiplies<size_t>());
    }

    inline auto LinearIndex(size_t i) const
    {
        return i;
    }

    inline auto LinearIndex(size_t y, size_t x) const
    {
        auto [_, width] = shape_;
        return y * width + x;
    }

    inline auto LinearIndex(size_t z, size_t y, size_t x) const
    {
        auto [_, height, width] = shape_;
        return z * (height * width) + y * width + x;
    }

    friend bool operator==(Tensor const& left, Tensor const& right)
    {
        // Expect(DimensionsEqual(left, right));
        for (size_t i = 0; i < left.size_; ++i)
        {
            if (left.data_[i] != right.data_[i])
            {
                return false;
            }
        }
        return true;
    }
};

#include "Arithmetic.hpp"
