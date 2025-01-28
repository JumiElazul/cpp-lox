#include "expression_visitors.h"
#include "expressions.h"
#include "typedefs.h"
#include <string>

NAMESPACE_BEGIN(lumina)

std::string string_visitor::visit_binary(const binary_expression& expr) const
{
    std::string lhs_str = expr.expr_lhs->accept_visitor(*this);
    std::string rhs_str = expr.expr_rhs->accept_visitor(*this);

    return lhs_str + expr.oper.lexeme + rhs_str;
}

std::string string_visitor::visit_unary(const unary_expression& expr) const
{
    std::string expr_str = expr.expr->accept_visitor(*this);
    return expr.oper.lexeme + expr_str;
}

NAMESPACE_END
