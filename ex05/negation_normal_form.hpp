#ifndef NEGATION_NORMAL_FORM_HPP
# define NEGATION_NORMAL_FORM_HPP

# include <string>

// Time complexity:  O(n)
// Space complexity: O(n)
//
// Returns the negation normal form of an RPN boolean formula:
//   - only !, &, | operators may appear in the result
//   - negation may only appear directly in front of a variable
// Requires the RPN parser/AST from ex03 (copy into this folder).
std::string negation_normal_form(const std::string& formula);

#endif
