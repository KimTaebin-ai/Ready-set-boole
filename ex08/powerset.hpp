#ifndef POWERSET_HPP
# define POWERSET_HPP

# include <vector>
# include <cstdint>

// Time complexity:  O(n * 2^n)
// Space complexity: O(n * 2^n)
//
// The subject caps the space at O(2^n), which counts subsets: there are
// exactly 2^n of them. Counting the integers they hold instead gives
// n * 2^(n-1), which no correct powerset can avoid, hence the figure above.
//
// Returns the powerset of the input set, i.e. the set of all its subsets.
// Output size is exactly 2^|set|.
// Throws std::length_error for a set too large to enumerate (see the cap in
// powerset.cpp); the input is otherwise assumed valid, with no duplicates.
std::vector<std::vector<int32_t>> powerset(const std::vector<int32_t>& set);

#endif
