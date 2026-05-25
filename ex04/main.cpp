#include <iostream>
#include "print_truth_table.hpp"

int main()
{
	// Subject example: "AB&C|"
	std::cout << "Truth table for AB&C|" << std::endl;
	print_truth_table("AB&C|");
	return 0;
}
