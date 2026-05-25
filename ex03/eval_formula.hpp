#ifndef EVAL_FORMULA_HPP
# define EVAL_FORMULA_HPP

# include <string>

// Time complexity:  O(n)   n = length of formula
// Space complexity: O(n)
//
// Evaluates an RPN (postfix) boolean formula.
// Tokens:
//   0 1            literals
//   !              negation        (unary)
//   & | ^ > =      and/or/xor/imply/equiv (binary)
//
// Throws std::invalid_argument on malformed input.
bool eval_formula(const std::string& formula);

#endif
