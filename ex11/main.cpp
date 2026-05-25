#include <iostream>
#include <iomanip>
#include "reverse_map.hpp"

static void test(double n)
{
	auto [x, y] = reverse_map(n);
	std::cout << "reverse_map(" << std::setw(20) << n << ") = ("
	          << x << ", " << y << ")" << std::endl;
}

int main()
{
	std::cout << std::fixed << std::setprecision(12);
	test(0.0);
	test(0.25);
	test(0.5);
	test(0.75);
	test(1.0);
	return 0;
}
