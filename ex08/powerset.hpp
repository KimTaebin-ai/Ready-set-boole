#ifndef POWERSET_HPP
# define POWERSET_HPP

# include <vector>
# include <cstdint>

// Time complexity:  O(n * 2^n)
// Space complexity: O(n * 2^n)
//
// Returns the powerset of the input set, i.e. the set of all its subsets.
// Output size is exactly 2^|set|.
std::vector<std::vector<int32_t>> powerset(const std::vector<int32_t>& set);

#endif
