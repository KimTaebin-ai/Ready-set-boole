#include "reverse_map.hpp"

std::pair<uint16_t, uint16_t> reverse_map(double n)
{
	(void)n;
	// TODO: multiply n by 2^32, cast to uint32_t, then de-interleave the
	// bits to recover the original x and y from ex10's map().
	return std::make_pair<uint16_t, uint16_t>(0, 0);
}
