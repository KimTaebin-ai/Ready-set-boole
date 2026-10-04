#ifndef MULTIPLIER_HPP
# define MULTIPLIER_HPP

# include <cstdint>

// Subject limit:    time O(1), space O(1)
// Time complexity:  O(1)   exactly 32 loop iterations, each calling adder at
//                   most once. adder is O(log n) but n is a u32, so it runs
//                   at most 32 times: the whole call is capped at 32 * 32
//                   adder iterations whatever a and b are.
// Space complexity: O(1)
// Allowed: bitwise ops (& | ^ << >>), comparison ops, and the adder from ex00.
// Forbidden: arithmetic ops (+ - * / %). ++ allowed only as a loop index.
uint32_t multiplier(uint32_t a, uint32_t b);

#endif
