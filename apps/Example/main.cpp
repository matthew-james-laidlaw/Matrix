#include "Matrix.hpp"

#include <iostream>

using namespace isp;

int main()
{
	try
	{
		Matrix<int> m1 = {
			{ 1, 2 },
			{ 3, 4 },
		};

		Matrix<int> m2 = {
			{ 4, 3 },
			{ 2, 1 },
		};

		auto m3 = m1 + m2;

		std::cout << m1 << std::endl;
		std::cout << m2 << std::endl;
		std::cout << m3 << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
