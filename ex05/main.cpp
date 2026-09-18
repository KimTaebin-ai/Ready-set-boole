#include <iostream>
#include <stdexcept>
#include "negation_normal_form.hpp"

static void test(const std::string& formula, const std::string& expected) {
	std::string got = negation_normal_form(formula);
	std::cout << "nnf(\"" << formula << "\") = \"" << got << "\""
	          << "  [expected \"" << expected << "\"]" << std::endl;
}

static void show(const std::string& formula) {
	std::cout << "nnf(\"" << formula << "\") = \""
	          << negation_normal_form(formula) << "\"" << std::endl;
}

static void test_invalid(const std::string& formula) {
	try {
		std::string got = negation_normal_form(formula);

		std::cout << "nnf(\"" << formula << "\") = \"" << got
		          << "\"  [expected a throw]" << std::endl;
	} catch (const std::invalid_argument& e) {
		std::cout << "nnf(\"" << formula << "\") threw: " << e.what()
		          << std::endl;
	}
}

int main() {
	// Subject reference examples:
	test("AB&!", "A!B!|");
	test("AB|!", "A!B!&");
	test("AB>",  "A!B|");
	test("AB=",  "AB&A!B!&|");
	test("AB|C&!", "A!B!&C!|");

	// Already in NNF, so nothing should change.
	test("A", "A");
	test("A!", "A!");
	test("AB&", "AB&");
	test("AB|", "AB|");
	test("A!B!|", "A!B!|");

	// Double negation collapses.
	test("A!!", "A");
	test("A!!!", "A!");
	test("AB&!!", "AB&");

	// The remaining operators, and negations pushed through them.
	std::cout << std::endl;
	show("AB^");
	show("AB^!");
	show("AB=!");
	show("AB>!");
	show("ABC||!");
	show("ABC&&!");
	show("AB>C>");
	show("AB&CD&|!");

	// Malformed input.
	std::cout << std::endl;
	test_invalid("");
	test_invalid("AB");
	test_invalid("A&");
	test_invalid("A$B&");
	return 0;
}
