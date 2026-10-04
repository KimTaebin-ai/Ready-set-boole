#ifndef NEGATION_NORMAL_FORM_HPP
# define NEGATION_NORMAL_FORM_HPP

# include <string>

// Subject limit:    none (N/A)
// Time complexity:  O(n) for formulas built from !, &, | and >: each of those
//                   rewrites its operands once (A > B becomes !A | B).
//                   = and ^ are rewritten by writing both operands twice, so
//                   every nesting level of them doubles the output:
//                   AB=C=D=... grows about 2x per operator, up to O(2^n).
// Space complexity: proportional to the size of the returned formula.
//
// Returns the negation normal form of an RPN boolean formula:
//   - only !, &, | operators may appear in the result
//   - negation may only appear directly in front of a variable
//
// Throws std::invalid_argument on malformed input.
// Uses the shared AST module and NNF rewrite (common/ast.*, common/nnf.*).
std::string negation_normal_form(const std::string& formula);

#endif
