#include "multiplier.hpp"
#include "adder.hpp"

uint32_t multiplier(uint32_t a, uint32_t b) {
	uint32_t result = 0;

	// Long multiplication in base 2: every bit set in b contributes a copy of
	// a shifted by that bit's position. Exactly 32 iterations, so O(1).
	for (unsigned int i = 0; i < 32; ++i) {
		if (((b >> i) & 1) == 1)
			result = adder(result, a << i);
	}
	return result;
}
