#include "expressions.h"
#include "expression_visitors.h"
#include "typedefs.h"

NAMESPACE_BEGIN(lumina)

std::string binary_expression::accept_visitor(const expression_visitor& v) const
{
    return v.visit_binary(*this);
}

std::string unary_expression::accept_visitor(const expression_visitor& v) const
{
    return v.visit_unary(*this);
}

NAMESPACE_END
