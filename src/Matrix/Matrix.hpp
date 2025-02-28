#pragma once

#include "MatrixInitializer.hpp"

#include <Number.hpp>
#include <Expect.hpp>

#include <array>
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

};

}
