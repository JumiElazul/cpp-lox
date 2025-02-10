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

assignment_expression::assignment_expression(const token& ident_name, std::unique_ptr<expression> initializer_expr)
    : ident_name(ident_name)
    , initializer_expr(std::move(initializer_expr)) { }

logical_expression::logical_expression(std::unique_ptr<expression> lhs_, token oper_, std::unique_ptr<expression> rhs_)
    : expr_lhs(std::move(lhs_))
    , oper(oper_)
    , expr_rhs(std::move(rhs_)) { }

literal_value unary_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_unary(*this); }
literal_value binary_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_binary(*this); }
literal_value ternary_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_ternary(*this); }
literal_value literal_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_literal(*this); }
literal_value grouping_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_grouping(*this); }
literal_value variable_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_variable(*this); }
literal_value assignment_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_assignment(*this); }
literal_value logical_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_logical(*this); }

void unary_expression::accept_visitor(expression_visitor<void>& v) { v.visit_unary(*this); }
void binary_expression::accept_visitor(expression_visitor<void>& v) { v.visit_binary(*this); }
void ternary_expression::accept_visitor(expression_visitor<void>& v) { v.visit_ternary(*this); }
void literal_expression::accept_visitor(expression_visitor<void>& v) { v.visit_literal(*this); }
void grouping_expression::accept_visitor(expression_visitor<void>& v) { v.visit_grouping(*this); }
void variable_expression::accept_visitor(expression_visitor<void>& v) { v.visit_variable(*this); }
void assignment_expression::accept_visitor(expression_visitor<void>& v) { v.visit_assignment(*this); }
void logical_expression::accept_visitor(expression_visitor<void>& v) { v.visit_logical(*this); }

std::string unary_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_unary(*this); }
std::string binary_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_binary(*this); }
std::string ternary_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_ternary(*this); }
std::string literal_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_literal(*this); }
std::string grouping_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_grouping(*this); }
std::string variable_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_variable(*this); }
std::string assignment_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_assignment(*this); }
std::string logical_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_logical(*this); }

NAMESPACE_END
