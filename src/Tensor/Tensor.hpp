#pragma once

#include "Number.hpp"

#include <array>
#include <memory>
#include <numeric>

template <size_t Order>
concept ValidOrder = (Order == 1 || Order == 2 || Order == 3);

template <size_t Order, typename... Dimensions>
concept ValidDimensions = (sizeof...(Dimensions) == Order) &&
                          (std::convertible_to<Dimensions, size_t> && ...);

template <Number T, size_t Order>
    requires ValidOrder<Order>
class Tensor
{
private:

    std::array<size_t, Order> shape_;
    std::unique_ptr<T[]> data_;

public:

    Tensor() = delete;

    ~Tensor() = default;

    Tensor(std::array<size_t, Order> const& shape)
        : shape_(shape)
        , data_(std::make_unique<T[]>(Size()))
    {}

    Tensor(std::array<size_t, Order> const& shape, T initializer)
        : Tensor(shape)
    {
        std::fill(data_.get(), data_.get() + Size(), initializer);
    }

    Tensor(std::array<size_t, Order> const& shape, std::initializer_list<T> const& initializer)
        : Tensor(shape)
    {
        if (initializer.size() != Size())
        {
            throw std::runtime_error("invalid tensor initializer list");
        }
        std::copy(initializer.begin(), initializer.end(), data_.get());
    }

    Tensor(Tensor const& other)
        : Tensor(other.shape_)
    {
        std::copy(other.data_.get(), other.data_.get() + Size(), data_.get());
    }

    Tensor(Tensor&& other)
        : shape_(other.shape_)
        , data_(std::move(other.data_))
    {}

    Tensor& operator=(Tensor const& other)
    {
        if (this == &other)
        {
            return *this;
        }

        shape_ = other.shape_;
        data_  = std::make_unique<T[]>(Size());
        std::copy(other.data_.get(), other.data_.get() + Size(), data_.get());

        return *this;
    }

    Tensor& operator=(Tensor&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        shape_ = other.shape_;
        data_  = std::move(other.data_);

        return *this;
    }

    auto Shape() const -> std::array<size_t, Order>
    {
        return shape_;
    }

    auto Size() const -> size_t
    {
        return std::accumulate(shape_.begin(), shape_.end(), size_t(1), std::multiplies<>());
    }

    auto Data() const -> const T* const
    {
        return data_.get();
    }

    template <typename... Indices>
        requires ValidDimensions<Order, Indices...>
    inline auto operator()(Indices... indices) -> T&
    {
        return data_[Linear(indices...)];
    }

    template <typename... Indices>
        requires ValidDimensions<Order, Indices...>
    inline auto operator()(Indices... indices) const -> T
    {
        return data_[Linear(indices...)];
    }

    friend bool operator==(Tensor const& left, Tensor const& right)
    {
        if (left.shape_ != right.shape_)
        {
            return false;
        }

        for (size_t i = 0; i < left.Size(); ++i)
        {
            if (left.data_[i] != right.data_[i])
            {
                return false;
            }
        }

        return true;
    }

private:

    inline auto Linear(size_t i) const -> size_t
    {
        return i;
    }

    inline auto Linear(size_t i, size_t j) const -> size_t
    {
        auto [_, d2] = shape_;
        return i * d2 + j;
    }

    inline auto Linear(size_t i, size_t j, size_t k) const -> size_t
    {
        auto [_, d2, d3] = shape_;
        return i * d2 * d3 + j * d3 + k;
    }
};

#include <Arithmetic.hpp>
