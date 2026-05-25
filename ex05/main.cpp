#include <iostream>
#include "negation_normal_form.hpp"

static void test(const std::string& formula, const std::string& expected)
{
	std::string got = negation_normal_form(formula);
	std::cout << "nnf(\"" << formula << "\") = \"" << got << "\""
	          << "  [expected \"" << expected << "\"]" << std::endl;
}

int main()
{
	// Subject reference examples:
	test("AB&!", "A!B!|");
	test("AB|!", "A!B!&");
	test("AB>",  "A!B|");
	test("AB=",  "AB&A!B!&|");
	test("AB|C&!", "A!B!&C!|");
	return 0;
}
