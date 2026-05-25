#include <iostream>
#include "eval_formula.hpp"

static void test(const std::string& formula, bool expected)
{
	bool got = eval_formula(formula);
	std::cout << "eval_formula(\"" << formula << "\") = "
	          << (got ? "true" : "false")
	          << "  [expected " << (expected ? "true" : "false") << "]"
	          << std::endl;
}

int main()
{
	test("10&", false);
	test("10|", true);
	test("11>", true);
	test("10=", false);
	test("1011||=", true);
	return 0;
}
