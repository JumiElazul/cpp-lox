#include "interpreter.h"
#include "exceptions.h"
#include "expressions.h"
#include "lumina_types.h"
#include "tokens.h"
#include "typedefs.h"

NAMESPACE_BEGIN(lumina)

literal_value interpreter::visit_unary(const unary_expression& expr) const
{
    return 1.0;
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
    throw std::runtime_error("Hit end in visit_binary");
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
