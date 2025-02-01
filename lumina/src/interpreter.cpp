#include "interpreter.h"
#include "exceptions.h"
#include "expressions.h"
#include "lumina_types.h"
#include "tokens.h"
#include "typedefs.h"

NAMESPACE_BEGIN(lumina)

literal_value interpreter::visit_unary(const unary_expression& expr) const
{
    literal_value literal = expr.expr_rhs->accept_visitor(*this);
    return handle_unary(expr.oper, literal);
}

literal_value interpreter::visit_binary(const binary_expression& expr) const
{
    literal_value lhs = expr.expr_lhs->accept_visitor(*this);
    literal_value rhs = expr.expr_rhs->accept_visitor(*this);

    return handle_binary(lhs, expr.oper, rhs);
}

literal_value interpreter::visit_ternary(const ternary_expression& expr) const
{
    literal_value if_expr = expr.expr_lhs->accept_visitor(*this);
    literal_value then_expr = expr.expr_lhs->accept_visitor(*this);
    literal_value else_expr = expr.expr_lhs->accept_visitor(*this);

    return handle_ternary(if_expr, expr.oper, then_expr, else_expr);
}

literal_value interpreter::visit_literal(const literal_expression& expr) const
{
    return expr.literal_val;
}

literal_value interpreter::visit_grouping(const grouping_expression& expr) const
{
    return expr.expr_lhs->accept_visitor(*this);
}

literal_value interpreter::handle_unary(const token& oper, const literal_value& literal) const
{
    lumina_type literal_type = literal_to_lumina_type(literal);

    switch (oper.type)
    {
        case token_type::bang_:
        {
            if (literal_type == lumina_type::bool_)
            {
                return !(std::get<bool>(literal));
            }
        } break;
        case token_type::minus_:
        {
            if (literal_type == lumina_type::number_)
            {
                return -(std::get<double>(literal));
            }
        } break;
        default:
        {
            throw interpreter_exception("Got to end in handle_unary()");
        } break;
    }
    throw interpreter_exception("Got to end in handle_unary()");
}

literal_value interpreter::handle_binary(const literal_value& lhs, const token& oper, const literal_value& rhs) const
{
    lumina_type lhs_type = literal_to_lumina_type(lhs);
    lumina_type rhs_type = literal_to_lumina_type(rhs);

    if (lhs_type != rhs_type)
    {
        std::string msg = std::string("Cannot use binary operator '" + oper.lexeme + "' on types " +
                lumina_type_tostr(lhs_type) + " and " + lumina_type_tostr(rhs_type));

        throw interpreter_exception(msg);
    }

    switch (oper.type)
    {
        case token_type::plus_:
        {
            if (lhs_type == lumina_type::string_)
            {
                return std::get<std::string>(lhs) + std::get<std::string>(rhs);
            }
            else if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) + std::get<double>(rhs);
            }
        } break;
        case token_type::minus_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) - std::get<double>(rhs);
            }
        } break;
        case token_type::star_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) * std::get<double>(rhs);
            }
        } break;
        case token_type::slash_:
        {
            if (lhs_type == lumina_type::number_)
            {
                return std::get<double>(lhs) / std::get<double>(rhs);
            }
        } break;
        default:
        {
            throw interpreter_exception("Unknown operator in visit_binary()");
        } break;
    }
    throw interpreter_exception("Got to end in handle_binary()");
}

literal_value interpreter::handle_ternary(const literal_value& if_literal, const token& oper, const literal_value& then_literal, const literal_value& else_literal) const
{
    lumina_type if_type = literal_to_lumina_type(if_literal);

    if (if_type != lumina_type::bool_)
    {
        throw interpreter_exception("Cannot convert lhs of ternary expression to bool");
    }

    return std::get<bool>(if_literal) ? then_literal : else_literal;
}

NAMESPACE_END
