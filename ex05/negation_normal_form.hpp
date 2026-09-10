#ifndef NEGATION_NORMAL_FORM_HPP
# define NEGATION_NORMAL_FORM_HPP

# include <string>

// Time complexity:  O(n) for !, & and |; exponential in the number of nested
//                   >, = and ^ operators, which are rewritten by duplicating
//                   their operands. The subject sets no limit here.
// Space complexity: same as the size of the returned formula.
//
// Returns the negation normal form of an RPN boolean formula:
//   - only !, &, | operators may appear in the result
//   - negation may only appear directly in front of a variable
//
// Throws std::invalid_argument on malformed input.
// Uses the shared AST module and NNF rewrite (common/ast.*, common/nnf.*).
std::string negation_normal_form(const std::string& formula);

#endif
