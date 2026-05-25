#ifndef REVERSE_MAP_HPP
# define REVERSE_MAP_HPP

# include <cstdint>
# include <utility>

// Time complexity:  O(1)
// Space complexity: O(1)
//
// Inverse of ex10's map(): takes a real n in [0, 1] and returns the (x, y)
// coordinates that map() would produce that value from.
// Precondition: 0.0 <= n <= 1.0.
std::pair<uint16_t, uint16_t> reverse_map(double n);

#endif
