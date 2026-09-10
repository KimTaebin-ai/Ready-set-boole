#include <iostream>
#include <stdexcept>
#include "conjunctive_normal_form.hpp"

static void test(const std::string& formula, const std::string& expected)
{
	std::string got = conjunctive_normal_form(formula);
	std::cout << "cnf(\"" << formula << "\") = \"" << got << "\""
	          << "  [expected \"" << expected << "\"]" << std::endl;
}

static void show(const std::string& formula)
{
	std::cout << "cnf(\"" << formula << "\") = \""
	          << conjunctive_normal_form(formula) << "\"" << std::endl;
}

static void test_invalid(const std::string& formula)
{
	try
	{
		std::string got = conjunctive_normal_form(formula);

		std::cout << "cnf(\"" << formula << "\") = \"" << got
		          << "\"  [expected a throw]" << std::endl;
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << "cnf(\"" << formula << "\") threw: " << e.what()
		          << std::endl;
	}
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

	// Already in CNF.
	test("A", "A");
	test("A!", "A!");
	test("AB&", "AB&");
	test("AB|", "AB|");

	// Distribution: (A & B) | C becomes (A | C) & (B | C).
	test("AB&C|", "AC|BC|&");

	// A | !A is a tautology, so the clause drops and nothing is left to
	// constrain the formula.
	test("AA!|", "1");
	// A & !A can never hold.
	test("AA!&", "AA!&");
	// Repeated literals in one clause collapse.
	test("AA|", "A");

	std::cout << std::endl;
	show("AB^");
	show("AB=");
	show("ABC^^");
	show("AB>C>");
	show("AB&CD&|");
	show("ABC||D&");

	// Malformed input.
	std::cout << std::endl;
	test_invalid("");
	test_invalid("AB");
	test_invalid("A&");
	test_invalid("A$B&");
	return 0;
}
