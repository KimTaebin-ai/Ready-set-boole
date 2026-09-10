#ifndef NNF_HPP
# define NNF_HPP

# include "ast.hpp"

// Returns an equivalent tree in negation normal form: only !, & and | are
// left, and every ! sits directly on a variable.
//
// Note that > and = (and ^) are rewritten by duplicating their operands, so
// a formula nesting those can grow exponentially. The subject sets no limit
// for this exercise.
NodePtr to_nnf(const Node& node);

#endif
