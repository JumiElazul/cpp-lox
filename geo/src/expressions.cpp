#include "expressions.h"
#include "expression_visitors.h"
#include "geo_types.h"
#include "typedefs.h"
#include <memory>

NAMESPACE_BEGIN(geo)

unary_expression::unary_expression(token oper, std::unique_ptr<expression> expr)
    : oper(oper)
    , expr_rhs(std::move(expr)) { }

binary_expression::binary_expression(std::unique_ptr<expression> lhs, token oper, std::unique_ptr<expression> rhs)
   : expr_lhs(std::move(lhs))
    , oper(oper)
    , expr_rhs(std::move(rhs)) { }

literal_expression::literal_expression(const literal_value& literal)
    : literal_val(literal) { }

grouping_expression::grouping_expression(std::unique_ptr<expression> expr)
    : expr_group(std::move(expr)) { }

variable_expression::variable_expression(const token& t)
    : ident_name(t) { }

assignment_expression::assignment_expression(const token& ident_name, std::unique_ptr<expression> initializer_expr)
    : ident_name(ident_name)
    , initializer_expr(std::move(initializer_expr)) { }

logical_expression::logical_expression(std::unique_ptr<expression> lhs_, token oper_, std::unique_ptr<expression> rhs_)
    : expr_lhs(std::move(lhs_))
    , oper(oper_)
    , expr_rhs(std::move(rhs_)) { }

postfix_expression::postfix_expression(std::unique_ptr<expression> expr_, token oper_)
    : expr_lhs(std::move(expr_))
    , oper(oper_) { }

call_expression::call_expression(std::unique_ptr<expression> callee_, token paren_, std::vector<std::unique_ptr<expression>> arguments_)
    : callee(std::move(callee_))
    , paren(paren_)
    , arguments(std::move(arguments_)) { }

get_expression::get_expression(std::unique_ptr<expression> object_, token name_)
    : object(std::move(object_))
    , name(name_) { }

set_expression::set_expression(std::unique_ptr<expression> object_, token name_, std::unique_ptr<expression> value_)
    : object(std::move(object_))
    , name(name_)
    , value(std::move(value_)) { }

this_expression::this_expression(const token& t)
    : keyword(t) { }

super_expression::super_expression(const token& keyword_, const token& method_)
    : keyword(keyword_), method(method_) { }

literal_value unary_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_unary(*this); }
literal_value binary_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_binary(*this); }
literal_value literal_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_literal(*this); }
literal_value grouping_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_grouping(*this); }
literal_value variable_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_variable(*this); }
literal_value assignment_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_assignment(*this); }
literal_value logical_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_logical(*this); }
literal_value postfix_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_postfix(*this); }
literal_value call_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_call(*this); }
literal_value get_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_get(*this); }
literal_value set_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_set(*this); }
literal_value this_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_this(*this); }
literal_value super_expression::accept_visitor(expression_visitor<literal_value>& v) { return v.visit_super(*this); }

void unary_expression::accept_visitor(expression_visitor<void>& v) { v.visit_unary(*this); }
void binary_expression::accept_visitor(expression_visitor<void>& v) { v.visit_binary(*this); }
void literal_expression::accept_visitor(expression_visitor<void>& v) { v.visit_literal(*this); }
void grouping_expression::accept_visitor(expression_visitor<void>& v) { v.visit_grouping(*this); }
void variable_expression::accept_visitor(expression_visitor<void>& v) { v.visit_variable(*this); }
void assignment_expression::accept_visitor(expression_visitor<void>& v) { v.visit_assignment(*this); }
void logical_expression::accept_visitor(expression_visitor<void>& v) { v.visit_logical(*this); }
void postfix_expression::accept_visitor(expression_visitor<void>& v) { v.visit_postfix(*this); }
void call_expression::accept_visitor(expression_visitor<void>& v) { v.visit_call(*this); }
void get_expression::accept_visitor(expression_visitor<void>& v) { v.visit_get(*this); }
void set_expression::accept_visitor(expression_visitor<void>& v) { v.visit_set(*this); }
void this_expression::accept_visitor(expression_visitor<void>& v) { v.visit_this(*this); }
void super_expression::accept_visitor(expression_visitor<void>& v) { v.visit_super(*this); }

std::string unary_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_unary(*this); }
std::string binary_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_binary(*this); }
std::string literal_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_literal(*this); }
std::string grouping_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_grouping(*this); }
std::string variable_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_variable(*this); }
std::string assignment_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_assignment(*this); }
std::string logical_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_logical(*this); }
std::string postfix_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_postfix(*this); }
std::string call_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_call(*this); }
std::string get_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_get(*this); }
std::string set_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_set(*this); }
std::string this_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_this(*this); }
std::string super_expression::accept_visitor(expression_visitor<std::string>& v) { return v.visit_super(*this); }

NAMESPACE_END
