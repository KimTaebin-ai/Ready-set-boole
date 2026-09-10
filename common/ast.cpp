#include "ast.hpp"

#include <stdexcept>

static std::invalid_argument error(const std::string& what, char token)
{
	return std::invalid_argument("parse_formula: " + what + " '" + token + "'");
}

NodePtr make_constant(bool value)
{
	NodePtr node = std::make_unique<Node>();

	node->kind = NodeKind::Constant;
	node->value = value;
	return node;
}

NodePtr make_variable(char name)
{
	NodePtr node = std::make_unique<Node>();

	node->kind = NodeKind::Variable;
	node->name = name;
	return node;
}

NodePtr make_not(NodePtr child)
{
	NodePtr node = std::make_unique<Node>();

	node->kind = NodeKind::Not;
	node->left = std::move(child);
	return node;
}

NodePtr make_binary(NodeKind kind, NodePtr left, NodePtr right)
{
	NodePtr node = std::make_unique<Node>();

	node->kind = kind;
	node->left = std::move(left);
	node->right = std::move(right);
	return node;
}

// Returns false for tokens that are not binary operators.
static bool binary_kind_of(char token, NodeKind& kind)
{
	switch (token)
	{
		case '&': kind = NodeKind::And;        return true;
		case '|': kind = NodeKind::Or;         return true;
		case '^': kind = NodeKind::Xor;        return true;
		case '>': kind = NodeKind::Implies;    return true;
		case '=': kind = NodeKind::Equivalent; return true;
		default:                               return false;
	}
}

static char symbol_of(NodeKind kind)
{
	switch (kind)
	{
		case NodeKind::And:     return '&';
		case NodeKind::Or:      return '|';
		case NodeKind::Xor:     return '^';
		case NodeKind::Implies: return '>';
		default:                return '=';   // Equivalent
	}
}

NodePtr parse_formula(const std::string& formula)
{
	std::vector<NodePtr> stack;

	for (char token : formula)
	{
		NodeKind kind;

		if (token == '0' || token == '1')
			stack.push_back(make_constant(token == '1'));
		else if (token >= 'A' && token <= 'Z')
			stack.push_back(make_variable(token));
		else if (token == '!')
		{
			if (stack.empty())
				throw error("missing operand for", token);

			NodePtr child = std::move(stack.back());

			stack.pop_back();
			stack.push_back(make_not(std::move(child)));
		}
		else if (binary_kind_of(token, kind))
		{
			if (stack.size() < 2)
				throw error("missing operand for", token);

			// The right operand was pushed last, so it pops first.
			NodePtr right = std::move(stack.back());

			stack.pop_back();

			NodePtr left = std::move(stack.back());

			stack.pop_back();
			stack.push_back(make_binary(kind, std::move(left),
				std::move(right)));
		}
		else
			throw error("unknown token", token);
	}
	if (stack.size() != 1)
		throw std::invalid_argument(
			"parse_formula: formula does not reduce to a single value");
	return std::move(stack.front());
}

static void append_rpn(const Node& node, std::string& out)
{
	switch (node.kind)
	{
		case NodeKind::Constant:
			out += node.value ? '1' : '0';
			return;

		case NodeKind::Variable:
			out += node.name;
			return;

		case NodeKind::Not:
			append_rpn(*node.left, out);
			out += '!';
			return;

		default:
			break;
	}
	append_rpn(*node.left, out);
	append_rpn(*node.right, out);
	out += symbol_of(node.kind);
}

std::string to_rpn(const Node& node)
{
	std::string rpn;

	append_rpn(node, rpn);
	return rpn;
}

bool evaluate(const Node& node, const Assignment& values)
{
	switch (node.kind)
	{
		case NodeKind::Constant: return node.value;
		case NodeKind::Variable: return values[node.name - 'A'];
		case NodeKind::Not:      return !evaluate(*node.left, values);
		default:                 break;
	}

	bool lhs = evaluate(*node.left, values);
	bool rhs = evaluate(*node.right, values);

	switch (node.kind)
	{
		case NodeKind::And:     return lhs && rhs;
		case NodeKind::Or:      return lhs || rhs;
		case NodeKind::Xor:     return lhs != rhs;
		case NodeKind::Implies: return !lhs || rhs;
		default:                return lhs == rhs;   // Equivalent
	}
}

static void mark_variables(const Node& node, Assignment& seen)
{
	if (node.kind == NodeKind::Variable)
		seen[node.name - 'A'] = true;
	if (node.left)
		mark_variables(*node.left, seen);
	if (node.right)
		mark_variables(*node.right, seen);
}

std::vector<char> variables_of(const Node& node)
{
	Assignment seen = {};
	std::vector<char> variables;

	mark_variables(node, seen);
	for (int i = 0; i < 26; ++i)
	{
		if (seen[i])
			variables.push_back(static_cast<char>('A' + i));
	}
	return variables;
}

Assignment assignment_of(const std::vector<char>& variables, size_t row)
{
	Assignment values = {};

	for (size_t i = 0; i < variables.size(); ++i)
	{
		size_t bit = variables.size() - 1 - i;

		values[variables[i] - 'A'] = ((row >> bit) & 1) == 1;
	}
	return values;
}
