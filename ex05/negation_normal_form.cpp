#include "negation_normal_form.hpp"
#include "ast.hpp"
#include "nnf.hpp"

std::string negation_normal_form(const std::string& formula)
{
	NodePtr tree = parse_formula(formula);
	NodePtr rewritten = to_nnf(*tree);

	return to_rpn(*rewritten);
}
