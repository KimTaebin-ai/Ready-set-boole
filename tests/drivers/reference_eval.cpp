#include "reference_eval.hpp"

#include <stdexcept>
#include <vector>

static bool is_binary_operator(char token)
{
	return token == '&' || token == '|' || token == '^'
	    || token == '>' || token == '=';
}

static bool apply_binary(char token, bool lhs, bool rhs)
{
	switch (token)
	{
		case '&': return lhs && rhs;                // conjunction
		case '|': return lhs || rhs;                // disjunction
		case '^': return lhs != rhs;                // exclusive disjunction
		case '>': return !lhs || rhs;               // material condition
		default:  return lhs == rhs;                // logical equivalence: '='
	}
}

static std::invalid_argument error(const std::string& what, char token)
{
	return std::invalid_argument("reference_eval: " + what + " '" + token + "'");
}

bool reference_eval(const std::string& formula)
{
	std::vector<bool> stack;

	for (char token : formula)
	{
		if (token == '0' || token == '1')
			stack.push_back(token == '1');
		else if (token == '!')
		{
			if (stack.empty())
				throw error("missing operand for", token);

			bool operand = stack.back();

			stack.pop_back();
			stack.push_back(!operand);
		}
		else if (is_binary_operator(token))
		{
			if (stack.size() < 2)
				throw error("missing operand for", token);

			// The right operand was pushed last, so it pops first.
			bool rhs = stack.back();

			stack.pop_back();

			bool lhs = stack.back();

			stack.pop_back();
			stack.push_back(apply_binary(token, lhs, rhs));
		}
		else
			throw error("unknown token", token);
	}
	if (stack.size() != 1)
		throw std::invalid_argument(
			"reference_eval: formula does not reduce to a single value");
	return stack.front();
}
