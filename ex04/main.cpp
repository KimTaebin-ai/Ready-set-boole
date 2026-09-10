#include <iostream>
#include "print_truth_table.hpp"

static void test(const std::string& formula)
{
	std::cout << "Truth table for " << formula << std::endl;
	print_truth_table(formula);
	std::cout << std::endl;
}

int main()
{
	// Subject example: (A & B) | C
	test("AB&C|");

	test("A");
	test("A!");
	test("AB=");
	test("AB>");
	test("AB^");
	test("ABC^^");

	// A formula with no variable at all still has a result column.
	test("10&");

	// Malformed input.
	test("AB&&");
	test("AB&Z");
	return 0;
}
