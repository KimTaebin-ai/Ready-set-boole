#include "print_truth_table.hpp"
#include "ast.hpp"

#include <iostream>
#include <stdexcept>

// One table row: "| 0 | 1 | 1 |", the result in the last cell.
static std::string row_of(const std::vector<char>& variables,
                          const Assignment& values, bool result)
{
	std::string row = "|";

	for (char variable : variables)
	{
		row += ' ';
		row += values[variable - 'A'] ? '1' : '0';
		row += " |";
	}
	row += result ? " 1 |" : " 0 |";
	return row;
}

void print_truth_table(const std::string& formula)
{
	NodePtr tree;

	// Parsing up front both validates the formula and means the rows below
	// only have to be evaluated, not parsed again.
	try
	{
		tree = parse_formula(formula);
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return;
	}

	std::vector<char> variables = variables_of(*tree);
	std::string header = "|";
	std::string separator = "|";

	for (char variable : variables)
	{
		header += ' ';
		header += variable;
		header += " |";
		separator += "---|";
	}
	std::cout << header << " = |" << std::endl;
	std::cout << separator << "---|" << std::endl;

	size_t rows = static_cast<size_t>(1) << variables.size();

	for (size_t row = 0; row < rows; ++row)
	{
		Assignment values = assignment_of(variables, row);

		std::cout << row_of(variables, values, evaluate(*tree, values))
		          << std::endl;
	}
}
