#include "eval_formula.hpp"
#include "ast.hpp"

#include <stdexcept>

bool eval_formula(const std::string& formula) {
	NodePtr tree = parse_formula(formula);

	// This exercise's formulas hold no variables, so there would be no value
	// to give one. The shared parser accepts them for the later exercises.
	if (!variables_of(*tree).empty())
		throw std::invalid_argument(
			"eval_formula: formula holds a variable, which has no value here");

	Assignment values = {};

	return evaluate(*tree, values);
}
