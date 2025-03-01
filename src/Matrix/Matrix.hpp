#pragma once

#include "MatrixInitializer.hpp"

#include <Number.hpp>
#include <Expect.hpp>

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace isp
{

template <Number T>
class Matrix
{
private:

	usize mHeight;
	usize mWidth;
	usize mSize;

	std::unique_ptr<T[]> mData;

public:

	Matrix() = delete;

	Matrix(usize height, usize width)
		: mHeight(height)
		, mWidth(width)
		, mSize(mHeight * mWidth)
		, mData(new T[mSize])
	{}

	Matrix(usize height, usize width, T initializer)
		: Matrix(height, width)
	{
		std::fill(mData.get(), mData.get() + mSize, initializer);
	}

	Matrix(MatrixInitializer<T> const& initializer)
	{
		std::tie(mHeight, mWidth) = Shape(initializer);
        mSize = mHeight * mWidth;
        mData = Flatten(initializer);
	}

	Matrix(Matrix const& other)
		: Matrix(other.mHeight, other.mWidth)
	{
		std::copy(other.mData.get(), other.mData.get() + mSize, mData.get());
	}

	Matrix(Matrix&& other)
		: mHeight(other.mHeight)
		, mWidth(other.mWidth)
		, mSize(other.mSize)
		, mData(std::move(other.mData))
	{
		other.mHeight = 0;
		other.mWidth = 0;
		other.mSize = 0;
	}

	Matrix& operator=(Matrix const& other)
	{
		if (this == &other)
		{
			return *this;
		}

		if (mSize != other.mSize)
		{
			mSize = other.mSize;
			mData.reset(new T[mSize]);
		}
	
		mHeight = other.mHeight;
		mWidth = other.mWidth;
	
		std::copy(other.mData.get(), other.mData.get() + mSize, mData.get());

		return *this;
	}

	Matrix& operator=(Matrix&& other)
	{
		if (this == &other)
		{
			return *this;
		}

		mHeight = other.mHeight;
		mWidth = other.mWidth;
		mSize = other.mSize;
		mData = std::move(other.mData);
	
		other.mHeight = 0;
		other.mWidth = 0;
		other.mSize = 0;
	
		return *this;
	}

	~Matrix() = default;

	usize Height() const
	{
		return mHeight;
	}

	usize Width() const
	{
		return mWidth;
	}

	usize Size() const
	{
		return mSize;
	}

	const T* Data() const
	{
		return mData.get();
	}

	inline T& operator()(usize y, usize x)
	{
		return mData[(y * mWidth) + x];
	}

	inline T operator()(usize y, usize x) const
	{
		return mData[(y * mWidth) + x];
	} 

	friend bool DimensionsEqual(Matrix const& left, Matrix const& right)
	{
		return left.Height() == right.Height() && left.Width() == right.Width();
	}

	template <typename Operation>
	friend Matrix ElementwiseOperation(Matrix const& left, Matrix const& right)
	{
		usize height = left.Height();
		usize width = left.Width();

		Matrix result(height, width);
		Operation operation;

		for (usize y = 0; y < height; ++y)
		{
			for (usize x = 0; x < width; ++x)
			{
				result(y, x) = operation(left(y, x), right(y, x));
			}
		}

		return result;
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
		for (usize y = 0; y < matrix.Height(); ++y)
		{
			out << "[ ";
			for (usize x = 0; x < matrix.Width(); ++x)
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

}
