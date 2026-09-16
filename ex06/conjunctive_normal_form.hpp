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
//
// The subject suggests reusing ex05 here, and the NNF step is shared with it
// through common/nnf.*. What this does not do is call ex05's
// negation_normal_form(), because that returns a formula as a string: going
// through it would mean serialising the tree and parsing it straight back.
// Both exercises call the same to_nnf() on the tree instead.
std::string conjunctive_normal_form(const std::string& formula);

#endif
