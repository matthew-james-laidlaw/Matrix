#pragma once

#include "Shape.hpp"
#include "Number.hpp"

#include <array>
#include <memory>
#include <numeric>

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
        , data_(std::make_unique<T[]>(shape.Size()))
    {}

    Tensor(std::array<size_t, Order> const& shape, T initializer)
        : Tensor(shape)
    {
        std::fill(data_.get(), data_.get() + shape.Size(), initializer);
    }

    Tensor(std::array<size_t, Order> const& shape, std::initializer_list<T> const& initializer)
        : Tensor(shape)
    {
        if (initializer.size() != shape.Size())
        {
            throw std::runtime_error("invalid tensor initializer list");
        }
        std::copy(initializer.begin(), initializer.end(), data_.get());
    }

    Tensor(Tensor const& other)
        : Tensor(other.shape_)
    {
        std::copy(other.data_.get(), other.data_.get() + shape_.Size(), data_.get());
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
        data_  = std::make_unique<T[]>(shape_.Size());
        std::copy(other.data_.get(), other.data_.get() + shape_.Size(), data_.get());

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
        std::accumulate(shape_.begin(), shape_.end(), size_t(1), std::multiplies<>());
    }

    template <typename... Indices>
        requires ValidDimensions<Indices...>
    inline auto operator()(Indices... indices) -> T&
    {
        return data_[Linear(indices...)];
    }

    template <typename... Indices>
        requires ValidDimensions<Indices...>
    inline auto operator()(Indices... indices) const -> T
    {
        return data_[Linear(indices...)];
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
