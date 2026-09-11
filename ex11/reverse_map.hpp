#ifndef REVERSE_MAP_HPP
# define REVERSE_MAP_HPP

# include <cstdint>
# include <utility>

// Time complexity:  O(1)
// Space complexity: O(1)
//
// Inverse of ex10's map(): takes a value map() produced and returns the
// (x, y) it came from, so reverse_map(map(x, y)) == (x, y) and
// map(reverse_map(n)) == n.
//
// The domain is the set map() actually produces, [0, (2^32 - 1) / 2^32].
// Note that this is not all of [0, 1]: map() divides a 32 bit index by 2^32,
// so 1.0 itself is never a result. Anything outside the domain throws
// std::out_of_range.
std::pair<uint16_t, uint16_t> reverse_map(double n);

#endif
