#include "negation_normal_form.hpp"

std::string negation_normal_form(const std::string& formula)
{
	(void)formula;
	// TODO: build an AST, rewrite >, =, ^ in terms of !, &, |, then
	// push negations down to the leaves via De Morgan's laws, then
	// serialise the AST back to RPN.
	return std::string();
}
