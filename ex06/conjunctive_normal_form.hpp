#ifndef CONJUNCTIVE_NORMAL_FORM_HPP
# define CONJUNCTIVE_NORMAL_FORM_HPP

# include <string>

// Time complexity:  O(2^n) worst-case (distribution can blow up)
// Space complexity: O(2^n) worst-case
//
// Returns the conjunctive normal form (clauses of disjunctions ANDed
// together) of an RPN boolean formula. Runs the shared NNF rewrite from ex05
// first, then distributes | over &.
// Uses the shared AST module and NNF rewrite (common/ast.*, common/nnf.*).
std::string conjunctive_normal_form(const std::string& formula);

#endif
