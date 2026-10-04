#ifndef EVAL_FORMULA_HPP
# define EVAL_FORMULA_HPP

# include <string>

// Subject limit:    time O(n), space N/A
// Time complexity:  O(n)   n = length of formula: one pass to parse, one walk
//                          to collect variables, one walk to evaluate
// Space complexity: O(n)   the tree holds one node per token
//
// Evaluates an RPN (postfix) boolean formula.
// Tokens:
//   0 1            literals
//   !              negation        (unary)
//   & | ^ > =      and/or/xor/imply/equiv (binary)
//
// Throws std::invalid_argument on malformed input.
// Uses the shared AST module (common/ast.*).
bool eval_formula(const std::string& formula);

#endif
