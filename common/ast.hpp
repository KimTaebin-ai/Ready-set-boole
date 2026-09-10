#ifndef AST_HPP
# define AST_HPP

# include <array>
# include <memory>
# include <string>
# include <vector>

// Shared RPN parser and abstract syntax tree for the formula exercises.
// Copied into each ex folder so every exercise compiles on its own.

enum class NodeKind
{
	Constant,
	Variable,
	Not,
	And,
	Or,
	Xor,
	Implies,
	Equivalent
};

struct Node;

using NodePtr = std::unique_ptr<Node>;

struct Node
{
	NodeKind kind = NodeKind::Constant;
	bool value = false;   // Constant only
	char name = 0;        // Variable only
	NodePtr left;         // unary operators use `left` on its own
	NodePtr right;
};

// One boolean per variable, indexed by name - 'A'.
using Assignment = std::array<bool, 26>;

NodePtr make_constant(bool value);
NodePtr make_variable(char name);
NodePtr make_not(NodePtr child);
NodePtr make_binary(NodeKind kind, NodePtr left, NodePtr right);

// Parses a formula in reverse polish notation. Tokens are 0, 1, A-Z, the
// unary ! and the binary & | ^ > =.
// Throws std::invalid_argument if the formula is malformed.
NodePtr parse_formula(const std::string& formula);

// Serialises the tree back to reverse polish notation.
std::string to_rpn(const Node& node);

bool evaluate(const Node& node, const Assignment& values);

// The variables the formula uses, in alphabetical order.
std::vector<char> variables_of(const Node& node);

// Assignment number `row`, counting from 0 to 2^variables.size() - 1. The
// first variable is the most significant bit, so walking `row` upwards lists
// the assignments in the order a truth table expects.
Assignment assignment_of(const std::vector<char>& variables, size_t row);

#endif
