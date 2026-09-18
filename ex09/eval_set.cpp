#include "eval_set.hpp"
#include "ast.hpp"

#include <set>
#include <stdexcept>
#include <string>

// The globally encompassing set, which the subject defines as the union of
// every set given as a parameter. Kept in order of first appearance so the
// result comes out in a predictable order.
static std::vector<int32_t> universe_of(
	const std::vector<std::vector<int32_t>>& sets) {
	std::set<int32_t> seen;
	std::vector<int32_t> universe;

	for (const std::vector<int32_t>& set : sets) {
		for (int32_t element : set) {
			if (seen.insert(element).second)
				universe.push_back(element);
		}
	}
	return universe;
}

std::vector<int32_t> eval_set(const std::string& formula,
                              const std::vector<std::vector<int32_t>>& sets) {
	NodePtr tree = parse_formula(formula);
	std::vector<char> variables = variables_of(*tree);

	// The subject asks for an error when the counts disagree.
	if (variables.size() != sets.size())
		throw std::invalid_argument("eval_set: the formula uses "
			+ std::to_string(variables.size()) + " variables but "
			+ std::to_string(sets.size()) + " sets were given");
	// A is the first set, B the second, and so on, so a variable's letter is
	// what picks its set. That only lines up while the letters run from A.
	for (char variable : variables) {
		if (static_cast<size_t>(variable - 'A') >= sets.size())
			throw std::invalid_argument(
				std::string("eval_set: no set matches variable '")
				+ variable + "', since A is the first set, B the second, ...");
	}

	std::vector<std::set<int32_t>> members;

	for (const std::vector<int32_t>& set : sets)
		members.push_back(std::set<int32_t>(set.begin(), set.end()));

	std::vector<int32_t> universe = universe_of(sets);
	std::vector<int32_t> result;

	// Decided one element at a time: asking which sets an element belongs to
	// gives a truth value per variable, and then the formula evaluator
	// handles &, |, !, ^, > and = as it always does. Since the walk only
	// visits the universe, ! comes out as the complement relative to it.
	for (int32_t element : universe) {
		// Indexed by letter, so it holds 26 slots. The count check above
		// bounds sets.size() to the number of distinct variables, which
		// cannot exceed 26, so the loop below stays inside the array.
		Assignment values = {};

		for (size_t i = 0; i < members.size(); ++i)
			values[i] = members[i].count(element) != 0;
		if (evaluate(*tree, values))
			result.push_back(element);
	}
	return result;
}
