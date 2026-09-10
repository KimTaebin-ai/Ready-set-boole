#ifndef SAT_HPP
# define SAT_HPP

# include <string>

// Time complexity:  O(2^k * n)   k = unique variables, n = formula length
// Space complexity: O(k + n)
//
// Returns true iff the RPN boolean formula is satisfiable, i.e. there is
// at least one assignment of its variables that makes it true.
// Brute-force is acceptable per the subject.
// Uses the shared AST module (common/ast.*).
bool sat(const std::string& formula);

#endif
