#include <iostream>
#include "adder.hpp"

int main()
{
	std::cout << "adder(0, 0)            = " << adder(0, 0) << std::endl;
	std::cout << "adder(1, 2)            = " << adder(1, 2) << std::endl;
	std::cout << "adder(42, 58)          = " << adder(42, 58) << std::endl;
	std::cout << "adder(123, 456)        = " << adder(123, 456) << std::endl;
	std::cout << "adder(0xFFFFFFFF, 1)   = " << adder(0xFFFFFFFFu, 1) << std::endl;
	std::cout << "adder(0x80000000, 0x80000000) = "
	          << adder(0x80000000u, 0x80000000u) << std::endl;
	return 0;
}
