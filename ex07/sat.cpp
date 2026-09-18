#include "sat.hpp"
#include "ast.hpp"

#include <vector>

bool sat(const std::string& formula) {
	NodePtr tree = parse_formula(formula);
	std::vector<char> variables = variables_of(*tree);
	size_t rows = static_cast<size_t>(1) << variables.size();

	// Brute force, as the subject allows: try every assignment and stop at
	// the first one that satisfies the formula.
	for (size_t row = 0; row < rows; ++row) {
		if (evaluate(*tree, assignment_of(variables, row)))
			return true;
	}
	return false;
}
