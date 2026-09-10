#ifndef ADDER_HPP
# define ADDER_HPP

# include <cstdint>

// Time complexity:  O(log n)   one iteration per carry propagation step
// Space complexity: O(1)
// Allowed: bitwise ops (& | ^ << >>) and comparison ops only.
// Forbidden: arithmetic ops (+ - * / %). ++ allowed only as a loop index.
uint32_t adder(uint32_t a, uint32_t b);

#endif
