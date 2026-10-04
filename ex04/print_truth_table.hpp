#ifndef PRINT_TRUTH_TABLE_HPP
# define PRINT_TRUTH_TABLE_HPP

# include <string>

// Subject limit:    time O(2^n), space N/A
// Time complexity:  O(2^k * n)   k = distinct variables, n = formula length.
//                   2^k rows, each evaluated in O(n). This is within O(2^n):
//                   a valid formula with k distinct variables has at least
//                   2k - 1 tokens, so k <= (n + 1) / 2 and 2^k * n is far
//                   below 2^n. Printing the table alone takes 2^k * k
//                   characters, so no correct version can drop the 2^k.
// Space complexity: O(k + n)     the tree plus one row at a time
//
// Prints the truth table for an RPN boolean formula.
// Variables are uppercase letters A-Z; operators are the same set as ex03.
// On malformed input it writes an error message to stderr and prints nothing.
// Uses the shared AST module (common/ast.*).
void print_truth_table(const std::string& formula);

#endif
