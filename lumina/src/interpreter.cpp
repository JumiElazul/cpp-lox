#include "interpreter.h"
#include "exceptions.h"
#include "expressions.h"
#include "lumina_types.h"
#include "tokens.h"
#include "typedefs.h"

NAMESPACE_BEGIN(lumina)

literal_value interpreter::visit_unary(const unary_expression& expr) const
{
    token_type oper_type = expr.oper.type;
    literal_value literal = expr.expr_rhs->accept_visitor(*this);

    lumina_type type = literal_to_lumina_type(literal);

    switch (oper_type)
    {
        case token_type::minus_:
        {
            if (type == lumina_type::number_)
            {
                return -(std::get<double>(literal));
            }
        } break;
        case token_type::bang_:
        {
            if (type == lumina_type::bool_)
            {
                return !(std::get<bool>(literal));
            }
        } break;
        default:
            throw interpreter_exception("Unknown operator in visit_unary()");
            break;
    }
    return "interpreter::visit_unary() failed";
}

literal_value interpreter::visit_binary(const binary_expression& expr) const
{
    literal_value lhs = expr.expr_lhs->accept_visitor(*this);
    literal_value rhs = expr.expr_rhs->accept_visitor(*this);

    switch (expr.oper.type)
    {
        case token_type::plus_:
        {

        } break;
        default:
            throw interpreter_exception("Unknown operator in visit_binary()");
            break;
    }
    return "interpreter::visit_binary() failed";
}

literal_value interpreter::visit_ternary(const ternary_expression& expr) const
{
    return 1.0;
}

literal_value interpreter::visit_literal(const literal_expression& expr) const
{
    return expr.literal_val;
}

literal_value interpreter::visit_grouping(const grouping_expression& expr) const
{
    return evaluate(*expr.expr_lhs);
}

literal_value interpreter::evaluate(const expression& expr) const
{
    return expr.accept_visitor(*this);
}

NAMESPACE_END
