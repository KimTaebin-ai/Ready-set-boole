#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "reverse_map.hpp"
#include "map.hpp"

static void show(double n)
{
	std::pair<uint16_t, uint16_t> point = reverse_map(n);

	std::cout << "reverse_map(" << std::setw(21) << n << ") = ("
	          << point.first << ", " << point.second << ")" << std::endl;
}

static void show_invalid(double n)
{
	try
	{
		std::pair<uint16_t, uint16_t> point = reverse_map(n);

		std::cout << "reverse_map(" << n << ") = (" << point.first << ", "
		          << point.second << ")  [expected a throw]" << std::endl;
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "reverse_map(" << n << ") threw: " << e.what()
		          << std::endl;
	}
}

int main()
{
	std::cout << std::setprecision(17);

	show(0.0);
	show(0.25);
	show(0.5);
	show(0.75);
	show(map(12345, 54321));
	show(map(65535, 65535));

	// (f^-1 . f)(x, y) == (x, y) over a 256x256 corner.
	size_t checked = 0;
	size_t mismatches = 0;

	for (uint32_t x = 0; x < 256; ++x)
	{
		for (uint32_t y = 0; y < 256; ++y)
		{
			uint16_t narrow_x = static_cast<uint16_t>(x);
			uint16_t narrow_y = static_cast<uint16_t>(y);
			std::pair<uint16_t, uint16_t> back =
				reverse_map(map(narrow_x, narrow_y));

			++checked;
			if (back.first != narrow_x || back.second != narrow_y)
				++mismatches;
		}
	}
	std::cout << std::endl
	          << "reverse_map(map(x, y)) == (x, y): " << checked
	          << " pairs, " << mismatches << " mismatches" << std::endl;

	// (f . f^-1)(n) == n for the values map() produces.
	size_t value_mismatches = 0;

	for (uint32_t i = 0; i < 65536; ++i)
	{
		double n = static_cast<double>(i) / 4294967296.0;
		std::pair<uint16_t, uint16_t> point = reverse_map(n);

		if (map(point.first, point.second) != n)
			++value_mismatches;
	}
	std::cout << "map(reverse_map(n)) == n: 65536 values, "
	          << value_mismatches << " mismatches" << std::endl;

	// 1.0 is in [0, 1] but map() never returns it, so it has no pair.
	std::cout << std::endl;
	show_invalid(1.0);
	show_invalid(-0.5);
	show_invalid(2.0);
	return 0;
}
