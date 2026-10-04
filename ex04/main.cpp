#include "print_truth_table.hpp"

// With no argument, `./ex04 | cat -e` writes exactly the subject's example
// table for (A & B) | C. Each argument given is printed as its own table:
//
//   ./ex04 'AB=' 'ABC^^' 'AB&&'
int main(int argc, char** argv) {
	if (argc < 2) {
		print_truth_table("AB&C|");
		return 0;
	}
	for (int i = 1; i < argc; ++i)
		print_truth_table(argv[i]);
	return 0;
}
