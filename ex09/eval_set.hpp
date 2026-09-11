#ifndef EVAL_SET_HPP
# define EVAL_SET_HPP

# include <string>
# include <vector>
# include <cstdint>

// Time complexity:  O(U * n + T log T)   U = universe size, n = formula
//                   length, T = total number of elements given
// Space complexity: O(T)
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
//
// Throws std::invalid_argument on a malformed formula, or when the number of
// sets does not match the number of variables the formula uses.
// Uses the shared AST module (common/ast.*).
std::vector<int32_t> eval_set(const std::string& formula,
                              const std::vector<std::vector<int32_t>>& sets);

#endif
