#include "Matrix.hpp"

#include <iostream>

using namespace isp;

int main()
{
	try
	{
		Matrix<int> matrix = {1, 2};
		std::cout << matrix.Height() << std::endl;
		std::cout << matrix.Width() << std::endl;
		std::cout << matrix.Size() << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
