#include "Matrix.hpp"
#include "Dispatcher.hpp"

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

		Matrix<int> m3(2, 2);

		DispatchBlocks(2, 2, [&](usize y, usize x)
		{
			m3(y, x) = m1(y, x) + m2(y, x);
		});

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
