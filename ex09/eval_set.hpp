#ifndef EVAL_SET_HPP
# define EVAL_SET_HPP

# include <string>
# include <vector>
# include <cstdint>

// Time complexity:  O(n * U)   n = formula length, U = universe size
// Space complexity: O(U)
//
// Evaluates an RPN boolean formula where uppercase variables A..Z are
// interpreted as sets (supplied via `sets`, indexed in formula order).
// Operators have set-theoretic meaning:
//   &  intersection
//   |  union
//   !  complement (relative to the union of all input sets)
//   ^  symmetric difference
//   >  material implication  (A> B)  ==  !A | B
//   =  equivalence
std::vector<int32_t> eval_set(const std::string& formula,
                              const std::vector<std::vector<int32_t>>& sets);

#endif
