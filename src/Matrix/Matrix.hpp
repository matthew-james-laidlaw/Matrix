#pragma once

#include "Dispatcher.hpp"
#include "MatrixInitializer.hpp"

#include <Number.hpp>
#include <Expect.hpp>

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>

enum class ThreadingPolicy
{
	Sequential,
	Threaded,
};

enum class ArithmeticPolicy
{
	Naive,
	Fused,
};

template <Number T>
class Matrix
{
private:

	static constexpr ThreadingPolicy threading_policy_ = ThreadingPolicy::Threaded;
	static constexpr ArithmeticPolicy arithmetic_policy_ = ArithmeticPolicy::Naive;

	size_t height_;
	size_t width_;
	size_t size_;

	std::unique_ptr<T[]> mData;

public:

	Matrix() = delete;

	Matrix(size_t height, size_t width)
		: height_(height)
		, width_(width)
		, size_(height_ * width_)
		, mData(new T[size_])
	{}

	Matrix(size_t height, size_t width, T initializer)
		: Matrix(height, width)
	{
		std::fill(mData.get(), mData.get() + size_, initializer);
	}

	Matrix(MatrixInitializer<T> const& initializer)
	{
		std::tie(height_, width_) = Shape(initializer);
        size_ = height_ * width_;
        mData = Flatten(initializer);
	}

	Matrix(Matrix const& other)
		: Matrix(other.height_, other.width_)
	{
		std::copy(other.mData.get(), other.mData.get() + size_, mData.get());
	}

	Matrix(Matrix&& other)
		: height_(other.height_)
		, width_(other.width_)
		, size_(other.size_)
		, mData(std::move(other.mData))
	{
		other.height_ = 0;
		other.width_ = 0;
		other.size_ = 0;
	}

	Matrix& operator=(Matrix const& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (size_ != other.size_)
		{
			size_ = other.size_;
			mData.reset(new T[size_]);
		}
	
		height_ = other.height_;
		width_ = other.width_;
	
		std::copy(other.mData.get(), other.mData.get() + size_, mData.get());

		return *this;
	}

	Matrix& operator=(Matrix&& other)
	{
		if (this == &other)
		{
			return *this;
		}

		height_ = other.height_;
		width_ = other.width_;
		size_ = other.size_;
		mData = std::move(other.mData);
	
		other.height_ = 0;
		other.width_ = 0;
		other.size_ = 0;
	
		return *this;
	}

	~Matrix() = default;

	size_t Height() const
	{
		return height_;
	}

	size_t Width() const
	{
		return width_;
	}

	size_t Size() const
	{
		return size_;
	}

	const T* Data() const
	{
		return mData.get();
	}

	inline T& operator()(size_t y, size_t x)
	{
		return mData[(y * width_) + x];
	}

	inline T operator()(size_t y, size_t x) const
	{
		return mData[(y * width_) + x];
	} 

	template <typename... Matrices>
	friend bool DimensionsEqual(Matrix const& first, Matrices... rest)
	{
		return ((first.Height() == rest.Height() && first.Width() == rest.Width()) && ...);
	}

	template <typename Operation>
	friend Matrix ElementwiseOperationSequential(Matrix const& left, Matrix const& right)
	{
		size_t height = left.Height();
		size_t width = left.Width();

		Matrix result(height, width);
		Operation operation;

		for (size_t y = 0; y < height; ++y)
		{
			for (size_t x = 0; x < width; ++x)
			{
				result(y, x) = operation(left(y, x), right(y, x));
			}
		}

		return result;
	}

	template <typename Operation>
	friend Matrix ElementwiseOperationThreaded(Matrix const& left, Matrix const& right)
	{
		size_t height = left.Height();
		size_t width = left.Width();

		Matrix result(height, width);
		Operation operation;

		DispatchBlocks(height, width, [&](size_t y, size_t x)
		{
			result(y, x) = operation(left(y, x), right(y, x));
		});

		return result;
	}

	template <typename Operation>
	friend Matrix ElementwiseOperation(Matrix const& left, Matrix const& right)
	{
		if constexpr (threading_policy_ == ThreadingPolicy::Sequential)
		{
			return ElementwiseOperationSequential<Operation>(left, right);
		}
		else if constexpr (threading_policy_ == ThreadingPolicy::Threaded)
		{
			return ElementwiseOperationThreaded<Operation>(left, right);
		}
		else
		{
			static_assert(false, "error: invalid threading policy");
		}
	}

	friend bool operator==(Matrix const& left, Matrix const& right)
	{
		Expect(DimensionsEqual(left, right));
	}

	friend Matrix operator+(Matrix const& left, Matrix const& right)
	{
		Expect(DimensionsEqual(left, right));
		return ElementwiseOperation<std::plus<T>>(left, right);
	}

	friend Matrix operator-(Matrix const& left, Matrix const& right)
	{
		Expect(DimensionsEqual(left, right));
		return ElementwiseOperation<std::minus<T>>(left, right);
	}

	friend Matrix operator*(Matrix const& left, Matrix const& right)
	{
		Expect(DimensionsEqual(left, right));
		return ElementwiseOperation<std::multiplies<T>>(left, right);
	}

	friend Matrix operator/(Matrix const& left, Matrix const& right)
	{
		Expect(DimensionsEqual(left, right));
		return ElementwiseOperation<std::divides<T>>(left, right);
	}

	friend std::ostream& operator<<(std::ostream& out, Matrix const& matrix)
	{
		for (size_t y = 0; y < matrix.Height(); ++y)
		{
			out << "[ ";
			for (size_t x = 0; x < matrix.Width(); ++x)
			{
				out << matrix(y, x);
				if (x != matrix.Width() - 1)
				{
					out << ", ";
				}
			}
			out << " ]";
			if (y != matrix.Height() - 1)
			{
				out << std::endl;
			}
		}
		return out;
	}

};
