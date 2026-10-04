#ifndef ADDER_HPP
# define ADDER_HPP

# include <cstdint>

// Subject limit:    time O(log n), space O(log n)
// Time complexity:  O(log n)   n = max(a, b). Each iteration moves the lowest
//                   pending carry at least one column left, and no carry can
//                   rise above the top bit of max(a, b) plus one, so the loop
//                   runs at most bitlen(max(a, b)) + 1 times (32 for u32).
// Space complexity: O(1)       three 32-bit words, whatever the input
// Allowed: bitwise ops (& | ^ << >>) and comparison ops only.
// Forbidden: arithmetic ops (+ - * / %). ++ allowed only as a loop index.
uint32_t adder(uint32_t a, uint32_t b);

#endif
