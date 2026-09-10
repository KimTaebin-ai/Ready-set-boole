// ex08 powerset: subject 예시 + 멱집합의 정의를 그대로 검사합니다.
#include "powerset.hpp"

#include <algorithm>
#include <cstdio>
#include <set>
#include <vector>

static int failures = 0;

using Subsets = std::vector<std::vector<int32_t>>;

static void check_exact(const std::vector<int32_t> &set, const Subsets &want)
{
	Subsets got = powerset(set);

	if (got != want)
	{
		std::printf("powerset of a %zu element set does not match the "
			"expected %zu subsets\n", set.size(), want.size());
		++failures;
	}
}

// |P(A)| == 2^n 이고, 원소가 모두 서로 다르며, 각 원소가 A 의 부분집합이면
// 서로 다른 부분집합 2^n 개 전체이므로 그것이 곧 멱집합입니다.
static void check_definition(const std::vector<int32_t> &set)
{
	Subsets got = powerset(set);
	size_t expected = static_cast<size_t>(1) << set.size();

	if (got.size() != expected)
	{
		std::printf("|powerset| of %zu elements: got %zu, want %zu\n",
			set.size(), got.size(), expected);
		++failures;
		return;
	}

	std::set<std::vector<int32_t>> distinct;
	std::vector<size_t> occurrences(set.size(), 0);

	for (const std::vector<int32_t> &subset : got)
	{
		std::vector<int32_t> sorted = subset;

		std::sort(sorted.begin(), sorted.end());
		if (std::unique(sorted.begin(), sorted.end()) != sorted.end())
		{
			std::printf("a subset repeats an element\n");
			++failures;
			return;
		}
		distinct.insert(sorted);
		for (int32_t element : subset)
		{
			auto found = std::find(set.begin(), set.end(), element);

			if (found == set.end())
			{
				std::printf("a subset holds %d, which is not in the input\n",
					element);
				++failures;
				return;
			}
			++occurrences[static_cast<size_t>(found - set.begin())];
		}
	}
	if (distinct.size() != expected)
	{
		std::printf("powerset of %zu elements holds duplicates: %zu "
			"distinct of %zu\n", set.size(), distinct.size(), expected);
		++failures;
	}
	// 각 원소는 나머지 원소의 부분집합 수, 즉 2^(n-1) 개에 들어갑니다.
	for (size_t i = 0; i < set.size(); ++i)
	{
		size_t want = expected / 2;

		if (occurrences[i] != want)
		{
			std::printf("element %d appears in %zu subsets, want %zu\n",
				set[i], occurrences[i], want);
			++failures;
		}
	}
}

int main()
{
	// 공집합의 멱집합은 비어 있지 않고, 공집합 하나를 담습니다.
	check_exact({}, Subsets{ {} });
	check_exact({1}, Subsets{ {}, {1} });
	check_exact({1, 2, 3}, Subsets{
		{}, {1}, {2}, {1, 2}, {3}, {1, 3}, {2, 3}, {1, 2, 3} });

	for (size_t n = 0; n <= 14; ++n)
	{
		std::vector<int32_t> set;

		for (size_t i = 0; i < n; ++i)
			set.push_back(static_cast<int32_t>(i) * 3 - 7);
		check_definition(set);
	}

	// 음수와 큰 값도 그냥 값일 뿐입니다.
	check_definition({-2147483647, 0, 2147483647});

	std::printf("sizes 0 to 14 checked against the definition, "
		"%d mismatches\n", failures);
	return failures != 0;
}
