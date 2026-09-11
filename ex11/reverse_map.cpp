#include "reverse_map.hpp"

#include <stdexcept>

// 2^32, matching the divisor map() used.
static const double index_count = 4294967296.0;

// The largest value map() can return, (2^32 - 1) / 2^32. Both the subtraction
// and the division are exact, so this is the precise upper end of the domain.
static const double largest = (index_count - 1.0) / index_count;

std::pair<uint16_t, uint16_t> reverse_map(double n)
{
	// Anything outside what map() produces has no pair to return. Rejecting
	// it also keeps the scaling below from overflowing a uint32_t.
	if (!(n >= 0.0 && n <= largest))
		throw std::out_of_range(
			"reverse_map: n must be in [0, (2^32 - 1) / 2^32], the values "
			"map() produces");

	// map() divided an exact 32 bit integer by a power of two, so scaling
	// back recovers that integer exactly. Rounding rather than truncating
	// means a value merely close to one map() produces still decodes.
	uint32_t index = static_cast<uint32_t>(n * index_count + 0.5);
	uint16_t x = 0;
	uint16_t y = 0;

	// Undo the interleaving: even bits rebuild x, odd bits rebuild y.
	for (unsigned int i = 0; i < 16; ++i)
	{
		if (((index >> (i * 2)) & 1) == 1)
			x = static_cast<uint16_t>(x | (1u << i));
		if (((index >> (i * 2 + 1)) & 1) == 1)
			y = static_cast<uint16_t>(y | (1u << i));
	}
	return std::make_pair(x, y);
}
