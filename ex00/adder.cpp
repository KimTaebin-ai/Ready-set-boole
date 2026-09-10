#include "adder.hpp"

uint32_t adder(uint32_t a, uint32_t b)
{
	// Ripple-carry addition: a ^ b is the sum of every column that does not
	// carry, a & b marks the columns that do. Shifting the carries one column
	// to the left and folding them back in repeats until nothing carries.
	while (b != 0)
	{
		uint32_t carry = a & b;

		a = a ^ b;
		b = carry << 1;
	}
	return a;
}
