#include "expression_visitors.h"
#include "expressions.h"
#include "lumina_types.h"
#include "console_io.h"
#include "typedefs.h"
#include "tokens.h"
#include <string>

NAMESPACE_BEGIN(lumina)

std::string string_visitor::visit_unary(const unary_expression& expr) const
{
    std::string expr_str = expr.expr_rhs->accept_visitor(*this);
    return expr.oper.lexeme + expr_str;
}

std::string string_visitor::visit_binary(const binary_expression& expr) const
{
    std::string lhs_str = expr.expr_lhs->accept_visitor(*this);
    std::string rhs_str = expr.expr_rhs->accept_visitor(*this);

    return lhs_str + " " + expr.oper.lexeme + " " + rhs_str;
}

std::string string_visitor::visit_ternary(const ternary_expression& expr) const
{
    std::string lhs_str = expr.expr_lhs->accept_visitor(*this);
    std::string then_str = expr.expr_then->accept_visitor(*this);
    std::string else_str = expr.expr_else->accept_visitor(*this);

    return lhs_str + " " + expr.oper.lexeme + " " + then_str + " : " + else_str;
}

std::string string_visitor::visit_literal(const literal_expression& expr) const
{
    return literal_tostr(expr.literal_val);
}

std::string string_visitor::visit_grouping(const grouping_expression& expr) const
{
    std::string lhs_str = expr.expr_lhs->accept_visitor(*this);
    return "( " + lhs_str + " )";
}

std::string string_visitor::visit_variable(const variable_expression& expr) const
{
    return expr.ident_name.lexeme;
}

NAMESPACE_END
