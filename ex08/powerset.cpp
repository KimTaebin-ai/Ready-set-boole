#include "powerset.hpp"

#include <stdexcept>

// Two reasons to cap the input. Shifting by the width of size_t or more is
// undefined, so the count has to stay below that. Well before it, the result
// stops fitting in memory: 2^n subsets at 24 bytes of vector each, plus
// n * 2^(n-1) integers, is already gigabytes around n = 25. Refusing with a
// clear error beats being killed by the allocator halfway through.
static const size_t max_elements = 25;

std::vector<std::vector<int32_t>> powerset(const std::vector<int32_t>& set) {
	if (set.size() > max_elements)
		throw std::length_error("powerset: a set of " +
			std::to_string(set.size()) + " elements has more subsets than can "
			"be held; at most " + std::to_string(max_elements) +
			" elements are supported");

	size_t subsets = static_cast<size_t>(1) << set.size();
	std::vector<std::vector<int32_t>> result;

	result.reserve(subsets);
	// Bit i of the mask says whether set[i] belongs to this subset, so
	// counting from 0 to 2^n - 1 walks every subset exactly once. Mask 0 is
	// the empty set, which is a subset of every set.
	for (size_t mask = 0; mask < subsets; ++mask) {
		std::vector<int32_t> subset;

		for (size_t i = 0; i < set.size(); ++i) {
			if (((mask >> i) & 1) == 1)
				subset.push_back(set[i]);
		}
		result.push_back(std::move(subset));
	}
	return result;
}
