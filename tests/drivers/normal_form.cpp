// ex05 negation_normal_form / ex06 conjunctive_normal_form.
// subject 예시값 + 의미 동등성 + NNF/CNF 구조 조건을 검사합니다.
#include "formula_util.hpp"

#ifdef TEST_CNF
# include "conjunctive_normal_form.hpp"
static const char *NAME = "cnf";
static std::string transform(const std::string &formula) {
	return conjunctive_normal_form(formula);
}
#else
# include "negation_normal_form.hpp"
static const char *NAME = "nnf";
static std::string transform(const std::string &formula) {
	return negation_normal_form(formula);
}
#endif

#include <cstdio>
#include <stdexcept>

static int failures = 0;

// NNF: 변수와 ! & | 만 나오고, 모든 ! 은 변수 바로 뒤여야 합니다.
//
// subject 는 결과가 변수와 ! & | 만 담도록 요구하므로, 입력에 상수가 없었다면
// 출력에도 0 이나 1 이 나와선 안 됩니다. allow_constants 는 subject 범위 밖인
// 상수 입력을 시험할 때만 켭니다.
static bool is_nnf(const std::string &formula, std::string &why,
	bool allow_constants) {
	for (size_t i = 0; i < formula.size(); ++i) {
		char token = formula[i];

		if (token >= 'A' && token <= 'Z')
			continue;
		if (token == '0' || token == '1') {
			if (allow_constants)
				continue;
			why = std::string("result holds the constant '") + token
				+ "', but only variables and ! & | are allowed";
			return false;
		}
		if (token == '&' || token == '|')
			continue;
		if (token == '!') {
			if (i == 0 || !(formula[i - 1] >= 'A' && formula[i - 1] <= 'Z')) {
				why = "'!' is not directly after a variable";
				return false;
			}
			continue;
		}
		why = std::string("disallowed symbol '") + token + "'";
		return false;
	}
	return true;
}

#ifdef TEST_CNF
// CNF: NNF 조건에 더해, | 안에 & 가 들어 있으면 안 됩니다. 스택 원소의
// 수준을 0=리터럴, 1=절, 2=논리곱으로 두고 | 의 피연산자를 확인합니다.
static bool is_cnf(const std::string &formula, std::string &why,
	bool allow_constants) {
	if (!is_nnf(formula, why, allow_constants))
		return false;

	std::vector<int> levels;

	for (char token : formula) {
		if ((token >= 'A' && token <= 'Z') || token == '0' || token == '1') {
			levels.push_back(0);
			continue;
		}
		if (token == '!')
			continue;   // 리터럴의 수준을 바꾸지 않습니다
		if (levels.size() < 2) {
			why = "malformed output";
			return false;
		}

		int rhs = levels.back();

		levels.pop_back();

		int lhs = levels.back();

		levels.pop_back();
		if (token == '|') {
			if (lhs == 2 || rhs == 2) {
				why = "'&' is nested inside a '|'";
				return false;
			}
			levels.push_back(1);
		} else
			levels.push_back(2);
	}
	if (levels.size() != 1) {
		why = "malformed output";
		return false;
	}
	return true;
}
#endif

static bool structure_ok(const std::string &formula, std::string &why,
	bool allow_constants) {
#ifdef TEST_CNF
	return is_cnf(formula, why, allow_constants);
#else
	return is_nnf(formula, why, allow_constants);
#endif
}

static void check_exact(const std::string &formula, const std::string &want) {
	std::string got = transform(formula);

	if (got != want) {
		std::printf("%s(\"%s\"): got \"%s\", want \"%s\"\n", NAME,
			formula.c_str(), got.c_str(), want.c_str());
		++failures;
	}
}

// 결과가 구조 조건을 만족하고 원식과 논리적으로 같은지 확인합니다.
static void check_formula(const std::string &formula) {
	std::string result;

	try {
		result = transform(formula);
	} catch (const std::exception &e) {
		std::printf("%s(\"%s\") threw: %s\n", NAME, formula.c_str(), e.what());
		++failures;
		return;
	}

	std::string why;
	// 입력에 상수가 있었다면 결과에 상수가 남는 것을 허용합니다.
	bool had_constants = formula.find('0') != std::string::npos
		|| formula.find('1') != std::string::npos;

	if (!structure_ok(result, why, had_constants)) {
		std::printf("%s(\"%s\") = \"%s\" is not in normal form: %s\n", NAME,
			formula.c_str(), result.c_str(), why.c_str());
		++failures;
	}
	if (!equivalent(formula, result)) {
		std::printf("%s(\"%s\") = \"%s\" is not equivalent to its input\n",
			NAME, formula.c_str(), result.c_str());
		++failures;
	}
}

static void check_throws(const std::string &formula) {
	try {
		std::string got = transform(formula);

		std::printf("%s(\"%s\"): returned \"%s\", want a throw\n", NAME,
			formula.c_str(), got.c_str());
		++failures;
	} catch (const std::invalid_argument &) {
	}
}

int main() {
	// subject 의 예시 전체.
#ifdef TEST_CNF
	check_exact("AB&!", "A!B!|");
	check_exact("AB|!", "A!B!&");
	check_exact("AB|C&", "AB|C&");
	check_exact("AB|C|D|", "ABCD|||");
	check_exact("AB&C&D&", "ABCD&&&");
	check_exact("AB&!C!|", "A!B!C!||");
	check_exact("AB|!C!&", "A!B!C!&&");
#else
	check_exact("AB&!", "A!B!|");
	check_exact("AB|!", "A!B!&");
	check_exact("AB>", "A!B|");
	check_exact("AB=", "AB&A!B!&|");
	check_exact("AB|C&!", "A!B!&C!|");
#endif

	// 이미 정규형인 입력은 그대로 남아야 합니다.
	check_exact("A", "A");
	check_exact("A!", "A!");
	check_exact("AB&", "AB&");
	check_exact("AB|", "AB|");

	// 이중 부정은 사라집니다.
	check_exact("A!!", "A");
	check_exact("A!!!", "A!");

	// 손으로 고른 입력들.
	const char *cases[] = {
		"A", "A!", "A!!", "AB&", "AB|", "AB^", "AB>", "AB=",
		"AB&!", "AB|!", "AB^!", "AB>!", "AB=!",
		"ABC||", "ABC&&", "ABC^^", "ABC>>", "ABC==",
		"AB&C|", "AB|C&", "AB&CD&|", "AB|CD|&",
		"AB>C>", "AB=C=", "ABC||!", "ABC&&!",
		"AA|", "AA&", "AA!|", "AA!&", "AA^", "AA=",
		"AB&AB&|", "AB&AB&!|", "ABCD&&&", "ABCD|||",
		"AB&C&D|", "AB|C|D&", "AB^C&", "AB=C|",
		"1", "0", "1!", "10&", "10|", "A1&", "A0|", "A1^"
	};

	for (const char *formula : cases)
		check_formula(formula);

	// 무작위 수식. & | ! > 만 쓰는 쪽은 깊게, 피연산자를 복제하는 = 와 ^ 가
	// 섞인 쪽은 폭발을 피하려고 얕게 생성합니다.
	std::mt19937 rng(20260910);

	for (int i = 0; i < RANDOM_ROUNDS; ++i)
		check_formula(random_formula(rng, 4, 4, "&|>"));
	for (int i = 0; i < RANDOM_ROUNDS; ++i)
		check_formula(random_formula(rng, 2, 3, "&|^>="));

	// 잘못된 입력.
	check_throws("");
	check_throws("AB");
	check_throws("A&");
	check_throws("!");
	check_throws("A$B&");

	std::printf("%d random formulas per operator set, %d mismatches\n",
		RANDOM_ROUNDS, failures);
	return failures != 0;
}
