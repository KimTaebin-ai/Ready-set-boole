#include "eval_set.hpp"

std::vector<int32_t> eval_set(const std::string& formula,
                              const std::vector<std::vector<int32_t>>& sets)
{
	(void)formula;
	(void)sets;
	// TODO: parse RPN, walk the AST, evaluate using set operations over the
	// universe = union of all input sets.
	return std::vector<int32_t>();
}
