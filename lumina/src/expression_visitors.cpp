#include "expression_visitors.h"
#include "expressions.h"
#include "lumina_types.h"
#include "console_io.h"
#include "typedefs.h"
#include "tokens.h"
#include <string>

NAMESPACE_BEGIN(lumina)

std::string string_visitor::visit_unary(unary_expression& expr)
{
    std::string expr_str = expr.expr_rhs->accept_visitor(*this);
    return expr.oper.lexeme + expr_str;
}

std::string string_visitor::visit_binary(binary_expression& expr)
{
    std::string lhs_str = expr.expr_lhs->accept_visitor(*this);
    std::string rhs_str = expr.expr_rhs->accept_visitor(*this);

    return lhs_str + " " + expr.oper.lexeme + " " + rhs_str;
}

std::string string_visitor::visit_ternary(ternary_expression& expr)
{
    std::string lhs_str = expr.expr_lhs->accept_visitor(*this);
    std::string then_str = expr.expr_then->accept_visitor(*this);
    std::string else_str = expr.expr_else->accept_visitor(*this);

    return lhs_str + " " + expr.oper.lexeme + " " + then_str + " : " + else_str;
}

std::string string_visitor::visit_literal(literal_expression& expr)
{
    return literal_tostr(expr.literal_val);
}

std::string string_visitor::visit_grouping(grouping_expression& expr)
{
    std::string lhs_str = expr.expr_->accept_visitor(*this);
    return "( " + lhs_str + " )";
}

std::string string_visitor::visit_variable(variable_expression& expr)
{
    return expr.ident_name.lexeme;
}

std::string string_visitor::visit_assignment(assignment_expression& expr)
{
    std::string lhs_str = expr.ident_name.lexeme;
    std::string rhs_str = expr.initializer_expr->accept_visitor(*this);

    return lhs_str + " = " + rhs_str;
}

std::string string_visitor::visit_logical(logical_expression& expr)
{
    std::string lhs_str = expr.expr_lhs->accept_visitor(*this);
    std::string rhs_str = expr.expr_rhs->accept_visitor(*this);

    return lhs_str + " " + expr.oper.lexeme + " " + rhs_str;
}

std::string string_visitor::visit_postfix(postfix_expression& expr)
{
    std::string lhs_str = expr.expr_lhs->accept_visitor(*this);
    return lhs_str + expr.oper.lexeme;
}

std::string string_visitor::visit_prefix(prefix_expression& expr)
{
    std::string rhs_str = expr.expr_rhs->accept_visitor(*this);
    return expr.oper.lexeme + rhs_str;
}

NAMESPACE_END
