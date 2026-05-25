#include <iostream>
#include "gray_code.hpp"

int main()
{
	// Subject reference values:
	// gray_code(0)=0 (1)=1 (2)=3 (3)=2 (4)=6 (5)=7 (6)=5 (7)=4 (8)=12
	for (uint32_t i = 0; i < 9; ++i)
		std::cout << "gray_code(" << i << ") = " << gray_code(i) << std::endl;
	return 0;
}
