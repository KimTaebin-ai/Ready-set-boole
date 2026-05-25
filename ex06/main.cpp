#include <iostream>
#include "conjunctive_normal_form.hpp"

static void test(const std::string& formula, const std::string& expected)
{
	std::string got = conjunctive_normal_form(formula);
	std::cout << "cnf(\"" << formula << "\") = \"" << got << "\""
	          << "  [expected \"" << expected << "\"]" << std::endl;
}

int main()
{
	// Subject reference examples:
	test("AB&!",   "A!B!|");
	test("AB|!",   "A!B!&");
	test("AB|C&",  "AB|C&");
	test("AB|C|D|",        "ABCD|||");
	test("AB&C&D&",        "ABCD&&&");
	test("AB&!C!|",        "A!B!C!||");
	test("AB|!C!&",        "A!B!C!&&");
	return 0;
}
