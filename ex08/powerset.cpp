#include "powerset.hpp"

std::vector<std::vector<int32_t>> powerset(const std::vector<int32_t>& set)
{
	size_t subsets = static_cast<size_t>(1) << set.size();
	std::vector<std::vector<int32_t>> result;

	result.reserve(subsets);
	// Bit i of the mask says whether set[i] belongs to this subset, so
	// counting from 0 to 2^n - 1 walks every subset exactly once. Mask 0 is
	// the empty set, which is a subset of every set.
	for (size_t mask = 0; mask < subsets; ++mask)
	{
		std::vector<int32_t> subset;

		for (size_t i = 0; i < set.size(); ++i)
		{
			if (((mask >> i) & 1) == 1)
				subset.push_back(set[i]);
		}
		result.push_back(std::move(subset));
	}
	return result;
}
