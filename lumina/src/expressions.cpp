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

variable_expression::variable_expression(const token& t)
    : ident_name(t) { }

literal_value unary_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_unary(*this); }
literal_value binary_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_binary(*this); }
literal_value ternary_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_ternary(*this); }
literal_value literal_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_literal(*this); }
literal_value grouping_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_grouping(*this); }
literal_value variable_expression::accept_visitor(const expression_visitor<literal_value>& v) const { return v.visit_variable(*this); }

void unary_expression::accept_visitor(const expression_visitor<void>& v) const { v.visit_unary(*this); }
void binary_expression::accept_visitor(const expression_visitor<void>& v) const { v.visit_binary(*this); }
void ternary_expression::accept_visitor(const expression_visitor<void>& v) const { v.visit_ternary(*this); }
void literal_expression::accept_visitor(const expression_visitor<void>& v) const { v.visit_literal(*this); }
void grouping_expression::accept_visitor(const expression_visitor<void>& v) const { v.visit_grouping(*this); }
void variable_expression::accept_visitor(const expression_visitor<void>& v) const { v.visit_variable(*this); }

std::string unary_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_unary(*this); }
std::string binary_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_binary(*this); }
std::string ternary_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_ternary(*this); }
std::string literal_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_literal(*this); }
std::string grouping_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_grouping(*this); }
std::string variable_expression::accept_visitor(const expression_visitor<std::string>& v) const { return v.visit_variable(*this); }

NAMESPACE_END
