#include <iostream>
#include "sat.hpp"

static void test(const std::string& formula, bool expected)
{
	bool got = sat(formula);
	std::cout << "sat(\"" << formula << "\") = "
	          << (got ? "true" : "false")
	          << "  [expected " << (expected ? "true" : "false") << "]"
	          << std::endl;
}

int main()
{
	// Subject reference examples:
	test("AB|",  true);
	test("AB&",  true);
	test("AA!&", false);
	test("AA^",  false);
	return 0;
}
