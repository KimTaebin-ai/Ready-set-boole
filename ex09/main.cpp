#include <iostream>
#include "eval_set.hpp"

static void print(const std::vector<int32_t>& s)
{
	std::cout << "{";
	for (size_t i = 0; i < s.size(); ++i) {
		if (i) std::cout << ",";
		std::cout << s[i];
	}
	std::cout << "}" << std::endl;
}

int main()
{
	// Subject reference examples:
	std::vector<std::vector<int32_t>> sets1 = {{0, 1, 2}, {0, 3, 4}};
	std::cout << "eval_set(\"AB&\", [{0,1,2},{0,3,4}]) = ";
	print(eval_set("AB&", sets1));

	std::vector<std::vector<int32_t>> sets2 = {{0, 1, 2}, {3, 4, 5}};
	std::cout << "eval_set(\"AB|\", [{0,1,2},{3,4,5}]) = ";
	print(eval_set("AB|", sets2));

	std::vector<std::vector<int32_t>> sets3 = {{0, 1, 2}};
	std::cout << "eval_set(\"A!\",  [{0,1,2}])         = ";
	print(eval_set("A!", sets3));
	return 0;
}
