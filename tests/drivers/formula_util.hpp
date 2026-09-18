#ifndef FORMULA_UTIL_HPP
# define FORMULA_UTIL_HPP

# include "reference_eval.hpp"

# include <random>
# include <string>
# include <vector>

// 수식 드라이버들이 공유하는 도우미. 평가는 모두 reference_eval 이 합니다.

using Values = std::vector<bool>;

// 수식 문자열에 등장하는 변수를 알파벳 순으로 모읍니다.
inline std::vector<char> variables_in(const std::string &formula) {
	bool seen[26] = {};
	std::vector<char> variables;

	for (char token : formula) {
		if (token >= 'A' && token <= 'Z')
			seen[token - 'A'] = true;
	}
	for (int i = 0; i < 26; ++i) {
		if (seen[i])
			variables.push_back(static_cast<char>('A' + i));
	}
	return variables;
}

// 변수를 값으로 바꾼 수식. 리터럴만 남으므로 그대로 평가할 수 있습니다.
inline std::string substitute(const std::string &formula,
	const Values &values) {
	std::string substituted = formula;

	for (char &token : substituted) {
		if (token >= 'A' && token <= 'Z')
			token = values[token - 'A'] ? '1' : '0';
	}
	return substituted;
}

inline bool eval_with(const std::string &formula, const Values &values) {
	return reference_eval(substitute(formula, values));
}

// row 번째 대입. 첫 변수가 최상위 비트입니다.
inline Values assignment_at(const std::vector<char> &variables, size_t row) {
	Values values(26, false);

	for (size_t i = 0; i < variables.size(); ++i) {
		size_t bit = variables.size() - 1 - i;

		values[variables[i] - 'A'] = ((row >> bit) & 1) == 1;
	}
	return values;
}

// 두 수식의 진리표가 모든 대입에서 같은지 확인합니다.
inline bool equivalent(const std::string &a, const std::string &b) {
	std::vector<char> variables = variables_in(a + b);
	size_t rows = static_cast<size_t>(1) << variables.size();

	for (size_t row = 0; row < rows; ++row) {
		Values values = assignment_at(variables, row);

		if (eval_with(a, values) != eval_with(b, values))
			return false;
	}
	return true;
}

// 무작위 RPN 수식. depth 로 크기를, ops 로 쓸 연산자를 제한합니다.
inline std::string random_formula(std::mt19937 &rng, int depth,
	unsigned int variable_count, const std::string &ops) {
	if (depth <= 0 || rng() % 4 == 0) {
		std::string leaf(1, static_cast<char>('A' + rng() % variable_count));

		if (rng() % 5 == 0)
			leaf += '!';
		return leaf;
	}
	if (rng() % 6 == 0)
		return random_formula(rng, depth - 1, variable_count, ops) + "!";

	std::string left = random_formula(rng, depth - 1, variable_count, ops);
	std::string right = random_formula(rng, depth - 1, variable_count, ops);

	return left + right + ops[rng() % ops.size()];
}

#endif
