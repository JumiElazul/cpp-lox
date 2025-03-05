#include "expression_visitors.h"
#include "expressions.h"
#include "geo_types.h"
#include "console_io.h"
#include "typedefs.h"
#include "tokens.h"
#include <string>

NAMESPACE_BEGIN(geo)

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

std::string string_visitor::visit_literal(literal_expression& expr)
{
    return literal_value_to_runtime_string(expr.literal_val);
}

std::string string_visitor::visit_grouping(grouping_expression& expr)
{
    std::string lhs_str = expr.expr_group->accept_visitor(*this);
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

std::string string_visitor::visit_call(call_expression& expr)
{
    std::string callee_str = expr.callee->accept_visitor(*this);
    std::string args_str = "";

    for (auto& arg : expr.arguments)
    {
        args_str += arg->accept_visitor(*this) + ", ";
    }

    if (!args_str.empty())
    {
        args_str.pop_back();
        args_str.pop_back();
    }

    return callee_str + "(" + args_str + ")";
}

NAMESPACE_END
