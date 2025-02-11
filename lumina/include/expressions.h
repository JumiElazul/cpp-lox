#ifndef JUMI_LUMINA_EXPRESSIONS_H
#define JUMI_LUMINA_EXPRESSIONS_H
#include "typedefs.h"
#include "tokens.h"
#include "lumina_types.h"
#include <memory>
#include <string>

NAMESPACE_BEGIN(lumina)

template<typename T>
class expression_visitor;

class expression
{
public:
    virtual ~expression() = default;
    virtual std::string accept_visitor(expression_visitor<std::string>& v) = 0;
    virtual void accept_visitor(expression_visitor<void>& v) = 0;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) = 0;
};

class unary_expression : public expression
{
public:
    token oper;
    std::unique_ptr<expression> expr_rhs;

    unary_expression(token oper, std::unique_ptr<expression> expr);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class binary_expression : public expression
{
public:
    std::unique_ptr<expression> expr_lhs;
    token oper;
    std::unique_ptr<expression> expr_rhs;

    binary_expression(std::unique_ptr<expression> lhs, token oper, std::unique_ptr<expression> rhs);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class ternary_expression : public expression
{
public:
    std::unique_ptr<expression> expr_lhs;
    token oper;
    std::unique_ptr<expression> expr_then;
    std::unique_ptr<expression> expr_else;

    ternary_expression(std::unique_ptr<expression> lhs, token oper, std::unique_ptr<expression> expr_then, std::unique_ptr<expression> expr_else);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class literal_expression : public expression
{
public:
    literal_value literal_val;

    literal_expression(const literal_value& literal);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class grouping_expression : public expression
{
public:
    std::unique_ptr<expression> expr_;

    grouping_expression(std::unique_ptr<expression> expr);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class variable_expression : public expression
{
public:
    token ident_name;

    variable_expression(const token& t);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class assignment_expression : public expression
{
public:
    token ident_name;
    std::unique_ptr<expression> initializer_expr;

    assignment_expression(const token& ident_name_, std::unique_ptr<expression> initializer_expr_);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class logical_expression : public expression
{
public:
    std::unique_ptr<expression> expr_lhs;
    token oper;
    std::unique_ptr<expression> expr_rhs;

    logical_expression(std::unique_ptr<expression> lhs_, token oper_, std::unique_ptr<expression> rhs_);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class postfix_expression : public expression
{
public:
    std::unique_ptr<expression> expr_lhs;
    token oper;

    postfix_expression(std::unique_ptr<expression> expr_, token oper_);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class prefix_expression : public expression
{
public:
    token oper;
    std::unique_ptr<expression> expr_rhs;

    prefix_expression(token oper, std::unique_ptr<expression> expr);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

NAMESPACE_END

#endif
