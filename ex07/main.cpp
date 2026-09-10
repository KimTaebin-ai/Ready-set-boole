#include <iostream>
#include <stdexcept>
#include "sat.hpp"

static void test(const std::string& formula, bool expected)
{
	bool got = sat(formula);
	std::cout << "sat(\"" << formula << "\") = "
	          << (got ? "true" : "false")
	          << "  [expected " << (expected ? "true" : "false") << "]"
	          << std::endl;
}

static void test_invalid(const std::string& formula)
{
	try
	{
		bool got = sat(formula);

		std::cout << "sat(\"" << formula << "\") = "
		          << (got ? "true" : "false")
		          << "  [expected a throw]" << std::endl;
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "sat(\"" << formula << "\") threw: " << e.what()
		          << std::endl;
	}
}

int main()
{
	// Subject reference examples:
	test("AB|",  true);
	test("AB&",  true);
	test("AA!&", false);
	test("AA^",  false);

	// Single variable.
	test("A",  true);
	test("A!", true);

	// Tautologies are satisfiable.
	test("AA!|", true);
	test("AA=",  true);
	test("AA>",  true);

	// Contradictions are not.
	test("AA=!",  false);
	test("AA!&B&", false);
	test("AB&AB&!&", false);

	// Constants.
	test("1", true);
	test("0", false);
	test("10&", false);
	test("10|", true);

	// Only one of the four assignments satisfies this, which is enough.
	test("AB&", true);
	test("A!B&", true);
	test("AB>!", true);

	// Malformed input.
	std::cout << std::endl;
	test_invalid("");
	test_invalid("AB");
	test_invalid("A&");
	test_invalid("A$B&");
	return 0;
}
