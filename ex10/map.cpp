#include "map.hpp"

// 2^32: the number of distinct interleaved indices, and the divisor that
// brings them into [0, 1).
static const double index_count = 4294967296.0;

// Spreads the 16 bits of `value` over every other bit position, so bit i
// lands at 2i and the gaps are left for the other coordinate.
static uint32_t spread(uint16_t value)
{
	uint32_t spread_bits = 0;

	for (unsigned int i = 0; i < 16; ++i)
	{
		if (((value >> i) & 1) == 1)
			spread_bits |= static_cast<uint32_t>(1) << (i * 2);
	}
	return spread_bits;
}

double map(uint16_t x, uint16_t y)
{
	// Z-order curve: interleaving the two coordinates uses each of the 2^32
	// indices exactly once, which is what makes the mapping bijective, and
	// it keeps points that are close in the plane close on the line.
	uint32_t index = spread(x) | (spread(y) << 1);

	// Exact, not approximate: the index carries at most 32 significant bits
	// against a 53 bit mantissa, and 2^32 is a power of two, so this only
	// changes the exponent. No two indices can land on the same double, and
	// reverse_map can scale back without losing anything.
	return static_cast<double>(index) / index_count;
}
