// ex04 print_truth_table: subject 의 예시 출력과 바이트 단위로 비교합니다.
// 기대 표는 코드가 아니라 논리에서 직접 손으로 유도한 것입니다.
#include "print_truth_table.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

static int failures = 0;

struct Capture
{
	std::string out;
	std::string err;
};

static Capture capture(const std::string &formula)
{
	std::ostringstream out;
	std::ostringstream err;
	std::streambuf *saved_out = std::cout.rdbuf(out.rdbuf());
	std::streambuf *saved_err = std::cerr.rdbuf(err.rdbuf());

	print_truth_table(formula);
	std::cout.rdbuf(saved_out);
	std::cerr.rdbuf(saved_err);

	Capture captured;

	captured.out = out.str();
	captured.err = err.str();
	return captured;
}

static void check(const std::string &formula, const std::string &want)
{
	Capture got = capture(formula);

	if (got.out != want)
	{
		std::printf("print_truth_table(\"%s\") wrote:\n%s\nwant:\n%s\n",
			formula.c_str(), got.out.c_str(), want.c_str());
		++failures;
	}
	if (!got.err.empty())
	{
		std::printf("print_truth_table(\"%s\") wrote to stderr: %s",
			formula.c_str(), got.err.c_str());
		++failures;
	}
}

static void check_invalid(const std::string &formula)
{
	Capture got = capture(formula);

	if (!got.out.empty())
	{
		std::printf("print_truth_table(\"%s\") is invalid but wrote a "
			"table:\n%s", formula.c_str(), got.out.c_str());
		++failures;
	}
	if (got.err.empty())
	{
		std::printf("print_truth_table(\"%s\") is invalid but wrote no "
			"error message\n", formula.c_str());
		++failures;
	}
}

int main()
{
	// subject 의 예시: (A & B) | C
	check("AB&C|",
		"| A | B | C | = |\n"
		"|---|---|---|---|\n"
		"| 0 | 0 | 0 | 0 |\n"
		"| 0 | 0 | 1 | 1 |\n"
		"| 0 | 1 | 0 | 0 |\n"
		"| 0 | 1 | 1 | 1 |\n"
		"| 1 | 0 | 0 | 0 |\n"
		"| 1 | 0 | 1 | 1 |\n"
		"| 1 | 1 | 0 | 1 |\n"
		"| 1 | 1 | 1 | 1 |\n");

	check("A",
		"| A | = |\n"
		"|---|---|\n"
		"| 0 | 0 |\n"
		"| 1 | 1 |\n");

	check("A!",
		"| A | = |\n"
		"|---|---|\n"
		"| 0 | 1 |\n"
		"| 1 | 0 |\n");

	check("AB&",
		"| A | B | = |\n"
		"|---|---|---|\n"
		"| 0 | 0 | 0 |\n"
		"| 0 | 1 | 0 |\n"
		"| 1 | 0 | 0 |\n"
		"| 1 | 1 | 1 |\n");

	check("AB|",
		"| A | B | = |\n"
		"|---|---|---|\n"
		"| 0 | 0 | 0 |\n"
		"| 0 | 1 | 1 |\n"
		"| 1 | 0 | 1 |\n"
		"| 1 | 1 | 1 |\n");

	check("AB^",
		"| A | B | = |\n"
		"|---|---|---|\n"
		"| 0 | 0 | 0 |\n"
		"| 0 | 1 | 1 |\n"
		"| 1 | 0 | 1 |\n"
		"| 1 | 1 | 0 |\n");

	// A ⇒ B: A 가 참이고 B 가 거짓일 때만 거짓.
	check("AB>",
		"| A | B | = |\n"
		"|---|---|---|\n"
		"| 0 | 0 | 1 |\n"
		"| 0 | 1 | 1 |\n"
		"| 1 | 0 | 0 |\n"
		"| 1 | 1 | 1 |\n");

	check("AB=",
		"| A | B | = |\n"
		"|---|---|---|\n"
		"| 0 | 0 | 1 |\n"
		"| 0 | 1 | 0 |\n"
		"| 1 | 0 | 0 |\n"
		"| 1 | 1 | 1 |\n");

	// 변수는 알파벳 순으로 열이 정해집니다. 등장 순서와 무관해야 합니다.
	check("BA>",
		"| A | B | = |\n"
		"|---|---|---|\n"
		"| 0 | 0 | 1 |\n"
		"| 0 | 1 | 0 |\n"
		"| 1 | 0 | 1 |\n"
		"| 1 | 1 | 1 |\n");

	// 같은 변수를 여러 번 써도 열은 하나입니다. A ^ A 는 항상 거짓.
	check("AA^",
		"| A | = |\n"
		"|---|---|\n"
		"| 0 | 0 |\n"
		"| 1 | 0 |\n");

	// 변수가 없어도 결과 열은 남습니다.
	check("10&",
		"| = |\n"
		"|---|\n"
		"| 0 |\n");

	// 잘못된 입력: 표를 절반만 출력하지 않고 오류만 남겨야 합니다.
	check_invalid("");
	check_invalid("AB&&");
	check_invalid("AB&Z");
	check_invalid("AB");
	check_invalid("A$B&");

	std::printf("%d mismatches\n", failures);
	return failures != 0;
}
