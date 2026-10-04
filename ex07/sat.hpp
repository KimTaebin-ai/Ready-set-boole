#ifndef SAT_HPP
# define SAT_HPP

# include <string>

// Subject limit:    time O(2^n), space N/A
// Time complexity:  O(2^k * n)   k = distinct variables, n = formula length.
//                   At most 2^k assignments, each evaluated in O(n). Within
//                   O(2^n) for the same reason as ex04: k <= (n + 1) / 2.
// Space complexity: O(k + n)     the tree plus one assignment at a time
//
// Returns true iff the RPN boolean formula is satisfiable, i.e. there is
// at least one assignment of its variables that makes it true.
// Brute-force is acceptable per the subject.
// Uses the shared AST module (common/ast.*).
bool sat(const std::string& formula);

#endif
