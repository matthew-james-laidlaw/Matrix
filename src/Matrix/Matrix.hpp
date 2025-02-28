#pragma once

#include <Number.hpp>
#include <Expect.hpp>

#include <array>
#include <initializer_list>
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

	Matrix(std::initializer_list<T> const& initializer)
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
		: Matrix(other.mHeight, other.mWidth)
	{
		mData = std::move(other.mData);
	}

	Matrix& operator=(Matrix const& other)
	{
		throw std::runtime_error("not implemented");
	}

	Matrix& operator=(Matrix&& other)
	{
		throw std::runtime_error("not implemented");
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
