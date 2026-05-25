#ifndef PRINT_TRUTH_TABLE_HPP
# define PRINT_TRUTH_TABLE_HPP

# include <string>

// Time complexity:  O(2^k * n)   k = unique variables, n = formula length
// Space complexity: O(k + n)
//
// Prints the truth table for an RPN boolean formula.
// Variables are uppercase letters A-Z; operators are the same set as ex03.
// Re-uses the RPN evaluator from ex03 (copy eval_formula.* into this folder
// when implementing, or factor a shared AST module).
void print_truth_table(const std::string& formula);

#endif
