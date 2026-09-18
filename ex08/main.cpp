#include <iostream>
#include "powerset.hpp"

static void print(const std::vector<std::vector<int32_t>>& ps) {
	std::cout << "[";
	for (size_t i = 0; i < ps.size(); ++i) {
		std::cout << "{";
		for (size_t j = 0; j < ps[i].size(); ++j) {
			if (j) std::cout << ",";
			std::cout << ps[i][j];
		}
		std::cout << "}";
		if (i + 1 < ps.size()) std::cout << ", ";
	}
	std::cout << "]" << std::endl;
}

int main() {
	std::vector<int32_t> a = {};
	std::vector<int32_t> b = {1};
	std::vector<int32_t> c = {1, 2, 3};
	std::vector<int32_t> d = {4, 8, 15, 16};
	std::vector<int32_t> e = {-1, 0, 7};

	// The powerset of the empty set is not empty: it holds the empty set.
	std::cout << "powerset({})        = "; print(powerset(a));
	std::cout << "powerset({1})       = "; print(powerset(b));
	std::cout << "powerset({1,2,3})   = "; print(powerset(c));
	std::cout << "powerset({4,8,15,16}) = "; print(powerset(d));
	std::cout << "powerset({-1,0,7})  = "; print(powerset(e));

	// |P(A)| = 2^|A|
	std::vector<int32_t> big;

	for (int32_t i = 0; i < 12; ++i)
		big.push_back(i);
	std::cout << std::endl
	          << "|powerset(0..11)| = " << powerset(big).size()
	          << "  [expected 4096]" << std::endl;
	return 0;
}
