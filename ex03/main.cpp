#include <iostream>
#include <stdexcept>
#include "eval_formula.hpp"

static void test(const std::string& formula, bool expected)
{
	bool got = eval_formula(formula);
	std::cout << "eval_formula(\"" << formula << "\") = "
	          << (got ? "true" : "false")
	          << "  [expected " << (expected ? "true" : "false") << "]"
	          << std::endl;
}

static void test_invalid(const std::string& formula)
{
	try
	{
		bool got = eval_formula(formula);

		std::cout << "eval_formula(\"" << formula << "\") = "
		          << (got ? "true" : "false") << "  [expected a throw]"
		          << std::endl;
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "eval_formula(\"" << formula << "\") threw: "
		          << e.what() << std::endl;
	}
}

int main()
{
	// Subject examples.
	test("10&", false);
	test("10|", true);
	test("11>", true);
	test("10=", false);
	test("1011||=", true);

	// Every operator, both ways.
	test("0", false);
	test("1", true);
	test("1!", false);
	test("0!!", false);
	test("11&", true);
	test("00|", false);
	test("10^", true);
	test("11^", false);
	test("10>", false);
	test("01>", true);
	test("11=", true);
	test("101|&", true);

	// Malformed input.
	test_invalid("");
	test_invalid("1&");
	test_invalid("!");
	test_invalid("11");
	test_invalid("1A&");
	return 0;
}
