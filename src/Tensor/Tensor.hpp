#pragma once

#include "Dispatcher.hpp"
// #include "TensorInitializer.hpp"

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
    std::unique_ptr<T[]> mData;

public:
    Tensor() = delete;

    template <typename... Dims>
    explicit Tensor(Dims... dims)
        : shape_{static_cast<size_t>(dims)...}, size_((... * dims)), mData(new T[size_])
    {}

    Tensor(std::array<size_t, N> shape, T initializer)
        : shape_(shape), size_(1), mData(new T[size_])
    {
        for (auto dim : shape_)
            size_ *= dim;
        std::fill(mData.get(), mData.get() + size_, initializer);
    }

    // Tensor(TensorInitializer<T, N> const &initializer)
    // {
    //     std::tie(shape_, size_) = Shape(initializer);
    //     mData = Flatten(initializer);
    // }

    Tensor(Tensor const &other)
        : shape_(other.shape_), size_(other.size_), mData(new T[size_])
    {
        std::copy(other.mData.get(), other.mData.get() + size_, mData.get());
    }

    Tensor(Tensor &&other) noexcept
        : shape_(std::move(other.shape_)), size_(other.size_), mData(std::move(other.mData))
    {
        other.size_ = 0;
    }

    Tensor &operator=(Tensor const &other)
    {
        if (this == &other)
            return *this;
        
        shape_ = other.shape_;
        size_ = other.size_;
        mData.reset(new T[size_]);
        std::copy(other.mData.get(), other.mData.get() + size_, mData.get());
        return *this;
    }

    Tensor &operator=(Tensor &&other) noexcept
    {
        if (this == &other)
            return *this;
        
        shape_ = std::move(other.shape_);
        size_ = other.size_;
        mData = std::move(other.mData);
        other.size_ = 0;
        return *this;
    }

    ~Tensor() = default;

    std::array<size_t, N> Shape() const { return shape_; }
    size_t Size() const { return size_; }
    const T *Data() const { return mData.get(); }

    size_t Index(std::array<size_t, N> indices) const
    {
        size_t index = 0, stride = 1;
        for (int i = N - 1; i >= 0; --i)
        {
            index += indices[i] * stride;
            stride *= shape_[i];
        }
        return index;
    }

    inline T &operator()(std::array<size_t, N> indices)
    {
        return mData[Index(indices)];
    }

    inline T operator()(std::array<size_t, N> indices) const
    {
        return mData[Index(indices)];
    }

    template <typename Operation, typename T1, typename T2>
    static auto ElementwiseOperation(Tensor<T1, N> const &left, Tensor<T2, N> const &right)
    {
        using ResultType = std::common_type_t<T1, T2>;
        Tensor<ResultType, N> result(left.shape_);
        Operation operation;
        DispatchBlocks(left.size_, [&](size_t i)
                       { result.mData[i] = operation(left.mData[i], right.mData[i]); });
        return result;
    }

    template <typename T1, typename T2>
    friend auto operator+(Tensor<T1, N> const &left, Tensor<T2, N> const &right)
    {
        Expect(DimensionsEqual(left, right));
        return ElementwiseOperation<std::plus<>, T1, T2>(left, right);
    }

    template <typename T1, typename T2>
    friend Tensor operator-(Tensor<T1, N> const &left, Tensor<T2, N> const &right)
    {
        Expect(DimensionsEqual(left, right));
        return ElementwiseOperation<std::minus<>, T1, T2>(left, right);
    }

    template <typename T1, typename T2>
    friend Tensor operator*(Tensor<T1, N> const &left, Tensor<T2, N> const &right)
    {
        Expect(DimensionsEqual(left, right));
        return ElementwiseOperation<std::multiplies<>, T1, T2>(left, right);
    }

    template <typename T1, typename T2>
    friend Tensor operator/(Tensor<T1, N> const &left, Tensor<T2, N> const &right)
    {
        Expect(DimensionsEqual(left, right));
        return ElementwiseOperation<std::divides<>, T1, T2>(left, right);
    }
};
