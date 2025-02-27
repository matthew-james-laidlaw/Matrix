#pragma once

#include "../Foundation.hpp"

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
	{
		throw std::runtime_error("not implemented");
	}

	Matrix(usize height, usize width, T initializer)
		: Matrix(height, width)
	{
		throw std::runtime_error("not implemented");
	}

	Matrix(std::initializer_list<T> const& initializer)
	{
		throw std::runtime_error("not implemented");
	}

	Matrix(Matrix const&)
	{
		throw std::runtime_error("not implemented");
	}

	Matrix(Matrix&&)
	{
		throw std::runtime_error("not implemented");
	}

	Matrix& operator=(Matrix const&)
	{
		throw std::runtime_error("not implemented");
	}

	Matrix& operator=(Matrix&&)
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
