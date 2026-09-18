#include <iomanip>
#include <iostream>
#include <set>
#include "map.hpp"

static void show(uint16_t x, uint16_t y) {
	std::cout << "map(" << std::setw(5) << x << ", " << std::setw(5) << y
	          << ") = " << map(x, y) << std::endl;
}

int main() {
	// 17 significant digits round-trip a double, so neighbouring values stay
	// visibly distinct.
	std::cout << std::setprecision(17);

	show(0, 0);
	show(1, 0);
	show(0, 1);
	show(1, 1);
	show(12345, 54321);
	show(65535, 65535);

	// The subject only requires the mapping to be bijective. Over a 256x256
	// corner that means 65536 coordinates giving 65536 distinct values, all
	// inside [0, 1].
	std::set<double> values;
	bool in_range = true;

	for (uint32_t x = 0; x < 256; ++x) {
		for (uint32_t y = 0; y < 256; ++y) {
			double value = map(static_cast<uint16_t>(x),
				static_cast<uint16_t>(y));

			if (!(value >= 0.0 && value <= 1.0))
				in_range = false;
			values.insert(value);
		}
	}
	std::cout << std::endl
	          << "256x256 grid: " << values.size()
	          << " distinct values  [expected 65536]" << std::endl
	          << "all within [0, 1]: " << (in_range ? "yes" : "no")
	          << std::endl;

	// The two coordinates must not be interchangeable, or the mapping would
	// collapse pairs onto one value.
	std::cout << "map(1, 0) == map(0, 1)? "
	          << (map(1, 0) == map(0, 1) ? "yes (broken)" : "no (correct)")
	          << std::endl;
	return 0;
}
