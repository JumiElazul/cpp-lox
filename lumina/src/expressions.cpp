#include "expressions.h"
#include "expression_visitors.h"
#include "typedefs.h"
#include <memory>

NAMESPACE_BEGIN(lumina)

binary_expression::binary_expression(std::unique_ptr<expression> lhs, token oper, std::unique_ptr<expression> rhs)
    : expr_lhs(std::move(lhs))
    , oper(oper)
    , expr_rhs(std::move(rhs))
{

}

literal_expression::literal_expression(const literal_value& literal)
    : literal_val(literal)
{ }

grouping_expression::grouping_expression(std::unique_ptr<expression> expr)
    : expr_lhs(std::move(expr))
{ }

unary_expression::unary_expression(token oper, std::unique_ptr<expression> expr)
    : oper(oper)
    , expr_rhs(std::move(expr))
{

}

std::string binary_expression::accept_visitor(const expression_visitor& v) const
{
    return v.visit_binary(*this);
}

std::string literal_expression::accept_visitor(const expression_visitor& v) const
{
    return v.visit_literal(*this);
}

std::string grouping_expression::accept_visitor(const expression_visitor& v) const
{
    return v.visit_grouping(*this);
}

std::string unary_expression::accept_visitor(const expression_visitor& v) const
{
    return v.visit_unary(*this);
}

NAMESPACE_END
