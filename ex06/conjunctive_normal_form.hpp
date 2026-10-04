#ifndef CONJUNCTIVE_NORMAL_FORM_HPP
# define CONJUNCTIVE_NORMAL_FORM_HPP

# include <string>

// Subject limit:    none (N/A)
// Time complexity:  exponential in the worst case. The NNF step can already
//                   double the formula at each nested = or ^ (see ex05), and
//                   distributing | over & multiplies clause counts on top of
//                   that. No tighter bound is claimed.
// Space complexity: exponential in the worst case. The result itself is
//                   bounded: duplicate and always-true clauses are dropped,
//                   so it holds at most 3^k distinct clauses for k variables
//                   (each variable appears plain, negated, or not at all).
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
