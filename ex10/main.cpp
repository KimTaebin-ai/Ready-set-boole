#include <iostream>
#include <iomanip>
#include "map.hpp"

int main()
{
	std::cout << std::fixed << std::setprecision(12);
	std::cout << "map(0, 0)         = " << map(0, 0) << std::endl;
	std::cout << "map(1, 0)         = " << map(1, 0) << std::endl;
	std::cout << "map(0, 1)         = " << map(0, 1) << std::endl;
	std::cout << "map(1, 1)         = " << map(1, 1) << std::endl;
	std::cout << "map(65535, 65535) = " << map(65535, 65535) << std::endl;
	std::cout << "map(12345, 54321) = " << map(12345, 54321) << std::endl;
	return 0;
}
