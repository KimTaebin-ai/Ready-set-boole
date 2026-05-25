#ifndef MAP_HPP
# define MAP_HPP

# include <cstdint>

// Time complexity:  O(1)   (bounded by 16 bit-pair iterations)
// Space complexity: O(1)
//
// Bijection N^2 -> [0, 1] using a space-filling curve. The point (x, y) is
// mapped to a deterministic real value in the unit interval such that the
// mapping is reversible by reverse_map (ex11). Implementation hint: build a
// 32-bit interleaved index from x and y (Z-order curve), then divide by 2^32.
double map(uint16_t x, uint16_t y);

#endif
