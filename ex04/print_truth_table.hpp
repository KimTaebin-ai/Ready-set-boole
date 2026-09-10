#ifndef PRINT_TRUTH_TABLE_HPP
# define PRINT_TRUTH_TABLE_HPP

# include <string>

// Time complexity:  O(2^k * n)   k = unique variables, n = formula length
// Space complexity: O(k + n)
//
// Prints the truth table for an RPN boolean formula.
// Variables are uppercase letters A-Z; operators are the same set as ex03.
// On malformed input it writes an error message to stderr and prints nothing.
// Uses the shared AST module (common/ast.*).
void print_truth_table(const std::string& formula);

#endif
