// ex09 eval_set: subject 예시 + 독립 기준 구현과의 대조.
#include "eval_set.hpp"
#include "formula_util.hpp"

#include <algorithm>
#include <cstdio>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

static int failures = 0;

using IntSet = std::set<int32_t>;

// 기준 구현. ex09 는 전체집합의 원소를 하나씩 훑으며 수식을 참/거짓으로
// 평가하지만, 여기서는 집합 자체를 스택에 쌓아 집합 연산으로 직접
// 계산합니다. 알고리즘이 달라 서로를 검증할 수 있습니다.
static IntSet complement_in(const IntSet& universe, const IntSet& operand) {
	IntSet out;

	std::set_difference(universe.begin(), universe.end(),
		operand.begin(), operand.end(), std::inserter(out, out.end()));
	return out;
}

static IntSet union_of(const IntSet& a, const IntSet& b) {
	IntSet out;

	std::set_union(a.begin(), a.end(), b.begin(), b.end(),
		std::inserter(out, out.end()));
	return out;
}

static IntSet intersection_of(const IntSet& a, const IntSet& b) {
	IntSet out;

	std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
		std::inserter(out, out.end()));
	return out;
}

static IntSet reference_eval_set(const std::string& formula,
	const std::vector<std::vector<int32_t>>& sets) {
	IntSet universe;

	for (const std::vector<int32_t>& set : sets)
		universe.insert(set.begin(), set.end());

	std::vector<IntSet> stack;

	for (char token : formula) {
		if (token >= 'A' && token <= 'Z') {
			size_t index = static_cast<size_t>(token - 'A');

			if (index >= sets.size())
				throw std::invalid_argument("reference: no set for variable");
			stack.push_back(IntSet(sets[index].begin(), sets[index].end()));
			continue;
		}
		if (token == '!') {
			IntSet operand = stack.back();

			stack.pop_back();
			stack.push_back(complement_in(universe, operand));
			continue;
		}

		IntSet rhs = stack.back();

		stack.pop_back();

		IntSet lhs = stack.back();

		stack.pop_back();
		switch (token) {
			case '&':
				stack.push_back(intersection_of(lhs, rhs));
				break;
			case '|':
				stack.push_back(union_of(lhs, rhs));
				break;
			case '^': {
				IntSet out;

				std::set_symmetric_difference(lhs.begin(), lhs.end(),
					rhs.begin(), rhs.end(), std::inserter(out, out.end()));
				stack.push_back(out);
				break;
			}
			case '>':
				// A > B is !A | B.
				stack.push_back(union_of(complement_in(universe, lhs), rhs));
				break;
			default:
				// A = B holds where both are in or both are out.
				stack.push_back(union_of(intersection_of(lhs, rhs),
					complement_in(universe, union_of(lhs, rhs))));
				break;
		}
	}
	return stack.back();
}

static void check(const std::string& formula,
	const std::vector<std::vector<int32_t>>& sets, const IntSet& want) {
	std::vector<int32_t> got;

	try {
		got = eval_set(formula, sets);
	} catch (const std::exception& e) {
		std::printf("eval_set(\"%s\") threw: %s\n", formula.c_str(), e.what());
		++failures;
		return;
	}

	IntSet as_set(got.begin(), got.end());

	if (as_set.size() != got.size()) {
		std::printf("eval_set(\"%s\") repeated an element\n", formula.c_str());
		++failures;
	}
	if (as_set != want) {
		std::printf("eval_set(\"%s\") does not match the expected set\n",
			formula.c_str());
		++failures;
	}
}

static void check_against_reference(const std::string& formula,
	const std::vector<std::vector<int32_t>>& sets) {
	try {
		std::vector<int32_t> got = eval_set(formula, sets);
		IntSet as_set(got.begin(), got.end());
		IntSet want = reference_eval_set(formula, sets);

		if (as_set.size() != got.size()) {
			std::printf("eval_set(\"%s\") repeated an element\n",
				formula.c_str());
			++failures;
		}
		if (as_set != want) {
			std::printf("eval_set(\"%s\"): %zu elements, reference says %zu\n",
				formula.c_str(), as_set.size(), want.size());
			++failures;
		}
	} catch (const std::exception& e) {
		std::printf("eval_set(\"%s\") threw: %s\n", formula.c_str(), e.what());
		++failures;
	}
}

static void check_throws(const std::string& formula,
	const std::vector<std::vector<int32_t>>& sets) {
	try {
		eval_set(formula, sets);
		std::printf("eval_set(\"%s\") returned, want a throw\n",
			formula.c_str());
		++failures;
	} catch (const std::invalid_argument&) {
	}
}

int main() {
	// subject 의 예시 전체.
	check("AB&", {{0, 1, 2}, {0, 3, 4}}, IntSet{0});
	check("AB|", {{0, 1, 2}, {3, 4, 5}}, IntSet{0, 1, 2, 3, 4, 5});
	check("A!", {{0, 1, 2}}, IntSet{});

	// 전체집합은 인자로 받은 집합들의 합집합입니다. 변수 개수와 집합 개수가
	// 같아야 하므로, 합집합이 드러나려면 수식이 두 변수를 모두 써야 합니다.
	// universe = {0,1,2}, A & B = {1}, 그 여집합 = {0,2}
	check("AB&!", {{0, 1}, {1, 2}}, IntSet{0, 2});
	// !A = {2}, 여기에 B 를 합치면 {1,2}
	check("A!B|", {{0, 1}, {1, 2}}, IntSet{1, 2});
	// 두 집합이 같으면 합집합이 곧 각 집합이라 여집합은 빕니다.
	check("AB|!", {{0, 1}, {0, 1}}, IntSet{});
	check("A", {{7, 8}}, IntSet{7, 8});
	check("AB^", {{0, 1, 2}, {1, 2, 3}}, IntSet{0, 3});
	check("AB>", {{0, 1}, {1, 2}}, IntSet{1, 2});
	check("AB=", {{0, 1}, {1, 2}}, IntSet{1});

	// 원소가 없는 집합, 중복 원소, 음수.
	check("AB|", {{}, {}}, IntSet{});
	check("AB&", {{-1, 0}, {0, 1}}, IntSet{0});

	// 개수가 맞지 않으면 오류입니다.
	check_throws("AB&", {{1}});
	// 변수가 하나뿐인 수식에 집합을 둘 주는 것도 개수 불일치입니다.
	check_throws("A!", {{0, 1, 2}, {3, 4}});
	check_throws("A", {{1}, {2}});
	check_throws("AC&", {{1}, {2}});
	check_throws("", {});
	check_throws("A&", {{1}});
	check_throws("A$B&", {{1}, {2}});

	// 무작위 수식 + 무작위 집합을 기준 구현과 대조합니다.
	std::mt19937 rng(20260910);
	int tested = 0;

	for (int round = 0; round < RANDOM_ROUNDS; ++round) {
		unsigned int count = 1 + rng() % 3;
		std::string formula = random_formula(rng, 3, count, "&|^>=");
		std::vector<char> variables = variables_in(formula);

		// ex09 는 변수 개수와 집합 개수가 같아야 하고, A 부터 차례로
		// 대응시키므로 변수가 A 부터 연속일 때만 유효한 입력입니다.
		bool contiguous = true;

		for (size_t i = 0; i < variables.size(); ++i) {
			if (variables[i] != static_cast<char>('A' + i))
				contiguous = false;
		}
		if (!contiguous || variables.empty())
			continue;

		std::vector<std::vector<int32_t>> sets;

		for (size_t i = 0; i < variables.size(); ++i) {
			std::vector<int32_t> set;

			// 겹침이 생기도록 좁은 값 범위에서 뽑습니다.
			for (int32_t element = 0; element < 8; ++element) {
				if (rng() % 2 == 0)
					set.push_back(element);
			}
			sets.push_back(set);
		}
		++tested;
		check_against_reference(formula, sets);
	}
	std::printf("%d random formulas vs. the set-algebra reference, "
		"%d mismatches\n", tested, failures);
	return failures != 0;
}
