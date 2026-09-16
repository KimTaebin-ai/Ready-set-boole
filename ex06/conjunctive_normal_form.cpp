#include "conjunctive_normal_form.hpp"
#include "ast.hpp"
#include "nnf.hpp"

#include <stdexcept>
#include <vector>

namespace
{

struct Literal
{
	char name;
	bool negated;
};

// A clause is a disjunction of literals; a CNF is a conjunction of clauses.
// The empty conjunction is true, and a conjunction holding an empty clause is
// false, so constants need no special case anywhere below.
using Clause = std::vector<Literal>;
using Clauses = std::vector<Clause>;

bool same_literal(const Literal& a, const Literal& b)
{
	return a.name == b.name && a.negated == b.negated;
}

// Adds `literal` unless the clause already holds it. Returns false when the
// clause already holds the opposite literal: A | !A is always true, so such a
// clause can be dropped from the conjunction entirely.
bool add_literal(Clause& clause, const Literal& literal)
{
	for (const Literal& kept : clause)
	{
		if (kept.name != literal.name)
			continue;
		return kept.negated == literal.negated;
	}
	clause.push_back(literal);
	return true;
}

// Joins two clauses into their disjunction. Returns false if the result is a
// tautology.
bool merge(const Clause& a, const Clause& b, Clause& out)
{
	out.clear();
	for (const Literal& literal : a)
	{
		if (!add_literal(out, literal))
			return false;
	}
	for (const Literal& literal : b)
	{
		if (!add_literal(out, literal))
			return false;
	}
	return true;
}

bool same_clause(const Clause& a, const Clause& b)
{
	if (a.size() != b.size())
		return false;
	// Neither clause repeats a literal, so equal sizes plus containment in
	// one direction is enough to call them the same set.
	for (const Literal& literal : a)
	{
		bool found = false;

		for (const Literal& other : b)
		{
			if (same_literal(literal, other))
			{
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}
	return true;
}

// Distribution produces the same clause over and over, so dropping duplicates
// as they are added keeps the intermediate results far smaller than filtering
// once at the end.
void add_clause(Clauses& clauses, const Clause& clause)
{
	for (const Clause& kept : clauses)
	{
		if (same_clause(kept, clause))
			return;
	}
	clauses.push_back(clause);
}

Clauses cnf_of(const Node& node)
{
	switch (node.kind)
	{
		case NodeKind::Constant:
			if (node.value)
				return Clauses();                    // true
			return Clauses(1, Clause());             // false

		case NodeKind::Variable:
			return Clauses(1, Clause(1, Literal{node.name, false}));

		case NodeKind::Not:
			// to_nnf() leaves negations sitting only on variables.
			return Clauses(1, Clause(1, Literal{node.left->name, true}));

		case NodeKind::And:
		{
			Clauses clauses = cnf_of(*node.left);
			Clauses right = cnf_of(*node.right);

			for (const Clause& clause : right)
				add_clause(clauses, clause);
			return clauses;
		}

		case NodeKind::Or:
		{
			// Distribute | over &: (x & y) | z becomes (x | z) & (y | z).
			Clauses left = cnf_of(*node.left);
			Clauses right = cnf_of(*node.right);
			Clauses clauses;

			for (const Clause& a : left)
			{
				for (const Clause& b : right)
				{
					Clause merged;

					if (merge(a, b, merged))
						add_clause(clauses, merged);
				}
			}
			return clauses;
		}

		default:
			throw std::logic_error(
				"conjunctive_normal_form: operator left over after NNF");
	}
}

// The subject requires the result to hold nothing but variables and the
// symbols ! & |, so the degenerate conjunctions cannot be written as "1" or
// "0". Both are expressible with a variable instead: X | !X always holds and
// X & !X never does. `spare` is a variable from the input, or 0 when the
// formula had none, which only happens for input the subject does not
// describe (a formula of bare constants).
std::string clauses_to_rpn(const Clauses& clauses, char spare)
{
	// No clauses left is the empty conjunction, which is always true. Dropping
	// tautological clauses above is what gets us here.
	if (clauses.empty())
	{
		if (spare == 0)
			return "1";
		return std::string(1, spare) + spare + "!|";
	}
	for (const Clause& clause : clauses)
	{
		// An empty clause is the empty disjunction, always false, so the whole
		// conjunction is false.
		if (clause.empty())
		{
			if (spare == 0)
				return "0";
			return std::string(1, spare) + spare + "!&";
		}
	}

	std::string rpn;

	// Every clause leaves exactly one value on the stack, so all the operands
	// can be written first and the operators folded in at the end:
	// "ABC||" is A | (B | C), and the & spine over the clauses works the same.
	for (const Clause& clause : clauses)
	{
		for (const Literal& literal : clause)
		{
			rpn += literal.name;
			if (literal.negated)
				rpn += '!';
		}
		rpn.append(clause.size() - 1, '|');
	}
	rpn.append(clauses.size() - 1, '&');
	return rpn;
}

}   // namespace

std::string conjunctive_normal_form(const std::string& formula)
{
	NodePtr tree = parse_formula(formula);
	std::vector<char> variables = variables_of(*tree);
	NodePtr rewritten = to_nnf(*tree);
	char spare = variables.empty() ? 0 : variables.front();

	return clauses_to_rpn(cnf_of(*rewritten), spare);
}
