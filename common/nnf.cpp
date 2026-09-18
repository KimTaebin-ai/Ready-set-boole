#include "nnf.hpp"

// `negated` carries a pending negation down the tree, so De Morgan's laws are
// applied on the way down rather than in a second pass over the result.
static NodePtr rewrite(const Node& node, bool negated);

// !(A = B) is A ^ B and !(A ^ B) is A = B, so both operators expand through
// the same shape; `exclusive` picks which one.
//
//   A = B  ->  (A & B) | (!A & !B)     both operands agree
//   A ^ B  ->  (A & !B) | (!A & B)     exactly one operand holds
static NodePtr expand_equivalence(const Node& node, bool exclusive) {
	NodePtr agree = make_binary(NodeKind::And,
		rewrite(*node.left, false), rewrite(*node.right, exclusive));
	NodePtr disagree = make_binary(NodeKind::And,
		rewrite(*node.left, true), rewrite(*node.right, !exclusive));

	return make_binary(NodeKind::Or, std::move(agree), std::move(disagree));
}

static NodePtr rewrite(const Node& node, bool negated) {
	switch (node.kind) {
		case NodeKind::Constant:
			return make_constant(negated ? !node.value : node.value);

		case NodeKind::Variable:
			if (negated)
				return make_not(make_variable(node.name));
			return make_variable(node.name);

		case NodeKind::Not:
			// Two negations cancel out instead of being emitted.
			return rewrite(*node.left, !negated);

		case NodeKind::And:
		case NodeKind::Or: {
			// De Morgan: a negation pushed through & comes out as |.
			bool is_and = node.kind == NodeKind::And;
			NodeKind kind = is_and != negated ? NodeKind::And : NodeKind::Or;

			return make_binary(kind, rewrite(*node.left, negated),
				rewrite(*node.right, negated));
		}

		case NodeKind::Implies:
			// A > B is !A | B, and its negation is A & !B.
			if (negated)
				return make_binary(NodeKind::And,
					rewrite(*node.left, false), rewrite(*node.right, true));
			return make_binary(NodeKind::Or,
				rewrite(*node.left, true), rewrite(*node.right, false));

		case NodeKind::Xor:
			return expand_equivalence(node, !negated);

		default:
			return expand_equivalence(node, negated);   // Equivalent
	}
}

NodePtr to_nnf(const Node& node) {
	return rewrite(node, false);
}
