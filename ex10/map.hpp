#ifndef MAP_HPP
# define MAP_HPP

# include <cstdint>

// Time complexity:  O(1)   (bounded by 16 bit-pair iterations)
// Space complexity: O(1)
//
// Maps a point of [0, 2^16 - 1]^2 to a value of [0, 1], injectively: every
// one of the 2^32 points gets its own value, so reverse_map (ex11) can undo
// it exactly.
//
// Done by interleaving the bits of x and y into a 32-bit Z-order index and
// dividing by 2^32. The division is exact rather than approximate, since a
// 32-bit index fits a 53-bit mantissa and the divisor is a power of two.
// Results therefore land in [0, (2^32 - 1) / 2^32], never on 1.0 itself.
double map(uint16_t x, uint16_t y);

#endif
