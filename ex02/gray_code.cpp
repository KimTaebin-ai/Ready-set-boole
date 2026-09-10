#include "gray_code.hpp"

uint32_t gray_code(uint32_t n)
{
	// Reflected binary code: bit i of the result is bit i of n flipped by its
	// left neighbour, which makes consecutive values differ by a single bit.
	return n ^ (n >> 1);
}
