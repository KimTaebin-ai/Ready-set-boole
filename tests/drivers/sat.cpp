// ex07 sat: subject 예시 + ex03 평가기로 구한 기준값과의 대조.
#include "formula_util.hpp"
#include "sat.hpp"

#include <cstdio>
#include <stdexcept>

static int failures = 0;

// 같은 명세를 다른 코드로 구현한 기준: 모든 대입을 훑어 참이 하나라도
// 있는지 봅니다. 평가는 ex03 의 스택 기반 평가기가 합니다.
static bool reference_sat(const std::string &formula) {
	std::vector<char> variables = variables_in(formula);
	size_t rows = static_cast<size_t>(1) << variables.size();

	for (size_t row = 0; row < rows; ++row) {
		if (eval_with(formula, assignment_at(variables, row)))
			return true;
	}
	return false;
}

static void check(const std::string &formula, bool want) {
	bool got = sat(formula);

	if (got != want) {
		std::printf("sat(\"%s\"): got %s, want %s\n", formula.c_str(),
			got ? "true" : "false", want ? "true" : "false");
		++failures;
	}
}

static void check_against_reference(const std::string &formula) {
	try {
		bool got = sat(formula);
		bool want = reference_sat(formula);

		if (got != want) {
			std::printf("sat(\"%s\"): got %s, reference says %s\n",
				formula.c_str(), got ? "true" : "false",
				want ? "true" : "false");
			++failures;
		}
	} catch (const std::exception &e) {
		std::printf("sat(\"%s\") threw: %s\n", formula.c_str(), e.what());
		++failures;
	}
}

static void check_throws(const std::string &formula) {
	try {
		bool got = sat(formula);

		std::printf("sat(\"%s\"): returned %s, want a throw\n",
			formula.c_str(), got ? "true" : "false");
		++failures;
	} catch (const std::invalid_argument &) {
	}
}

int main() {
	// subject 의 예시 전체.
	check("AB|", true);
	check("AB&", true);
	check("AA!&", false);
	check("AA^", false);

	// 항진식은 충족 가능, 모순식은 불가능.
	check("A", true);
	check("A!", true);
	check("AA!|", true);
	check("AA=", true);
	check("AA>", true);
	check("AA=!", false);
	check("AA!&B|", true);
	check("AA!&B&", false);
	check("AB&AB&!&", false);
	check("1", true);
	check("0", false);
	check("10&", false);

	const char *cases[] = {
		"AB^", "AB>", "AB=", "AB&!", "AB|!", "ABC||", "ABC&&",
		"ABC^^", "AB&C|", "AB|C&", "AA^B^", "AA=A=",
		"AB&A!&", "AB|A!&B!&", "AB=AB^&", "AB=AB=!&"
	};

	for (const char *formula : cases)
		check_against_reference(formula);

	std::mt19937 rng(20260910);

	for (int i = 0; i < RANDOM_ROUNDS; ++i)
		check_against_reference(random_formula(rng, 4, 4, "&|^>="));

	check_throws("");
	check_throws("AB");
	check_throws("A&");
	check_throws("A$B&");

	std::printf("%d random formulas vs. reference, %d mismatches\n",
		RANDOM_ROUNDS, failures);
	return failures != 0;
}
