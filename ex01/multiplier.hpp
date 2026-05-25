#ifndef MULTIPLIER_HPP
# define MULTIPLIER_HPP

# include <cstdint>

// Time complexity:  O(1)   (bounded by 32 bit shifts)
// Space complexity: O(1)
// Allowed: bitwise ops (& | ^ << >>), comparison ops, and the adder from ex00.
// Forbidden: arithmetic ops (+ - * / %). ++ allowed only as a loop index.
uint32_t multiplier(uint32_t a, uint32_t b);

#endif
