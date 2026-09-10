// ex03 eval_formula: subject 예시 + 각 연산자의 진리값 + 잘못된 입력 처리 +
// 독립 기준 평가기와의 대조.
#include "eval_formula.hpp"
#include "formula_util.hpp"

#include <cstdio>
#include <random>
#include <stdexcept>
#include <string>

static int failures = 0;

static void check(const std::string &formula, bool want)
{
	try
	{
		bool got = eval_formula(formula);

		if (got != want)
		{
			std::printf("eval_formula(\"%s\"): got %s, want %s\n",
				formula.c_str(), got ? "true" : "false",
				want ? "true" : "false");
			++failures;
		}
	}
	catch (const std::exception &e)
	{
		std::printf("eval_formula(\"%s\") threw unexpectedly: %s\n",
			formula.c_str(), e.what());
		++failures;
	}
}

static void check_throws(const std::string &formula)
{
	try
	{
		bool got = eval_formula(formula);

		std::printf("eval_formula(\"%s\"): returned %s, want a throw\n",
			formula.c_str(), got ? "true" : "false");
		++failures;
	}
	catch (const std::invalid_argument &)
	{
	}
	catch (const std::exception &e)
	{
		std::printf("eval_formula(\"%s\"): threw %s, want "
			"std::invalid_argument\n", formula.c_str(), e.what());
		++failures;
	}
}

int main()
{
	// subject 의 예시 전체.
	check("10&", false);
	check("10|", true);
	check("11>", true);
	check("10=", false);
	check("1011||=", true);

	// 리터럴과 부정.
	check("0", false);
	check("1", true);
	check("0!", true);
	check("1!", false);
	check("0!!", false);

	// 이항 연산자 네 가지 입력 전부.
	const char *conjunction[4] = { "00&", "01&", "10&", "11&" };
	const bool conjunction_want[4] = { false, false, false, true };
	const char *disjunction[4] = { "00|", "01|", "10|", "11|" };
	const bool disjunction_want[4] = { false, true, true, true };
	const char *exclusive[4] = { "00^", "01^", "10^", "11^" };
	const bool exclusive_want[4] = { false, true, true, false };
	const char *condition[4] = { "00>", "01>", "10>", "11>" };
	const bool condition_want[4] = { true, true, false, true };
	const char *equivalence[4] = { "00=", "01=", "10=", "11=" };
	const bool equivalence_want[4] = { true, false, false, true };

	for (int i = 0; i < 4; ++i)
	{
		check(conjunction[i], conjunction_want[i]);
		check(disjunction[i], disjunction_want[i]);
		check(exclusive[i], exclusive_want[i]);
		check(condition[i], condition_want[i]);
		check(equivalence[i], equivalence_want[i]);
	}

	// 피연산자 순서: '>' 는 좌우가 다르므로 스택 순서 오류를 잡아냅니다.
	check("10>", false);
	check("01>", true);

	// 중첩.
	check("101|&", true);
	check("110|&", true);
	check("100|&", false);
	check("11&1&", true);
	check("10&1|", true);

	// 잘못된 입력.
	check_throws("");
	check_throws("!");
	check_throws("&");
	check_throws("1&");
	check_throws("11");
	check_throws("1A&");
	check_throws("1 0&");
	check_throws("102&");

	// 무작위 수식의 모든 대입을 리터럴로 치환해, 공유 AST 를 쓰는
	// eval_formula 와 독립 스택 평가기의 결과가 같은지 대조합니다.
	std::mt19937 rng(20260910);
	long long compared = 0;

	for (int i = 0; i < RANDOM_ROUNDS; ++i)
	{
		std::string formula = random_formula(rng, 3, 3, "&|^>=");
		std::vector<char> variables = variables_in(formula);
		size_t rows = static_cast<size_t>(1) << variables.size();

		for (size_t row = 0; row < rows; ++row)
		{
			std::string literals =
				substitute(formula, assignment_at(variables, row));
			bool want = reference_eval(literals);
			bool got = eval_formula(literals);

			++compared;
			if (got != want)
			{
				std::printf("eval_formula(\"%s\"): got %s, reference says "
					"%s\n", literals.c_str(), got ? "true" : "false",
					want ? "true" : "false");
				++failures;
			}
		}
	}
	std::printf("%lld formulas vs. the independent evaluator, %d mismatches\n",
		compared, failures);
	return failures != 0;
}
