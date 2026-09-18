#include <iostream>
#include "multiplier.hpp"

int main() {
	std::cout << "multiplier(0, 0)     = " << multiplier(0, 0) << std::endl;
	std::cout << "multiplier(1, 2)     = " << multiplier(1, 2) << std::endl;
	std::cout << "multiplier(6, 7)     = " << multiplier(6, 7) << std::endl;
	std::cout << "multiplier(42, 58)   = " << multiplier(42, 58) << std::endl;
	std::cout << "multiplier(123, 456) = " << multiplier(123, 456) << std::endl;
	return 0;
}
