#ifndef POWERSET_HPP
# define POWERSET_HPP

# include <vector>
# include <cstdint>

// Subject limit:    time N/A, space O(2^n)
// Time complexity:  O(n * 2^n)
// Space complexity: O(2^n) subsets, which is what the subject's limit counts.
//                   Counted in integers, the returned subsets hold
//                   n * 2^(n-1) of them, and no correct powerset can return
//                   fewer. Nothing is allocated beyond that: the outer
//                   vector is reserved to exactly 2^n slots, each subset to
//                   exactly its own size, and each is moved into place.
//
// Returns the powerset of the input set, i.e. the set of all its subsets.
// Output size is exactly 2^|set|.
// Throws std::length_error for a set too large to enumerate (see the cap in
// powerset.cpp); the input is otherwise assumed valid, with no duplicates.
std::vector<std::vector<int32_t>> powerset(const std::vector<int32_t>& set);

#endif
