#include <iostream>
#include "powerset.hpp"

static void print(const std::vector<std::vector<int32_t>>& ps)
{
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

int main()
{
	std::vector<int32_t> a = {};
	std::vector<int32_t> b = {1};
	std::vector<int32_t> c = {1, 2, 3};

	std::cout << "powerset({})        = "; print(powerset(a));
	std::cout << "powerset({1})       = "; print(powerset(b));
	std::cout << "powerset({1,2,3})   = "; print(powerset(c));
	return 0;
}
