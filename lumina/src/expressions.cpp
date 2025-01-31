#include "expressions.h"
#include "expression_visitors.h"
#include "lumina_types.h"
#include "typedefs.h"
#include <memory>

NAMESPACE_BEGIN(lumina)


unary_expression::unary_expression(token oper, std::unique_ptr<expression> expr)
    : oper(oper)
    , expr_rhs(std::move(expr)) { }

binary_expression::binary_expression(std::unique_ptr<expression> lhs, token oper, std::unique_ptr<expression> rhs)
   : expr_lhs(std::move(lhs))
    , oper(oper)
    , expr_rhs(std::move(rhs)) { }

ternary_expression::ternary_expression(std::unique_ptr<expression> lhs, token oper,
        std::unique_ptr<expression> expr_then, std::unique_ptr<expression> expr_else)
    : expr_lhs(std::move(lhs))
    , oper(oper)
    , expr_then(std::move(expr_then))
    , expr_else(std::move(expr_else)) { }

literal_expression::literal_expression(const literal_value& literal)
    : literal_val(literal) { }

grouping_expression::grouping_expression(std::unique_ptr<expression> expr)
    : expr_lhs(std::move(expr)) { }

literal_value unary_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_unary(*this); }
literal_value binary_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_binary(*this); }
literal_value ternary_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_ternary(*this); }
literal_value literal_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_literal(*this); }
literal_value grouping_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_grouping(*this); }
std::string unary_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_unary(*this); }
std::string binary_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_binary(*this); }
std::string ternary_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_ternary(*this); }
std::string literal_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_literal(*this); }
std::string grouping_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_grouping(*this); }

NAMESPACE_END
