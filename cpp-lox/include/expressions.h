#ifndef JUMI_CPPLOX_EXPRESSIONS_H
#define JUMI_CPPLOX_EXPRESSIONS_H
#include "typedefs.h"
#include "tokens.h"
#include "cpplox_types.h"
#include <memory>
#include <string>
#include <vector>

NAMESPACE_BEGIN(cpplox)

template<typename T>
class expression_visitor;

class expression
{
public:
    virtual ~expression() = default;
    virtual std::string accept_visitor(expression_visitor<std::string>& v) = 0;
    virtual void accept_visitor(expression_visitor<void>& v) = 0;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) = 0;

protected:
    expression() = default;
    expression(const expression&) = default;
    expression& operator=(const expression&) = default;
    expression(expression&&) = default;
    expression& operator=(expression&&) = default;
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
    std::unique_ptr<expression> expr_group;

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

class call_expression : public expression
{
public:
    std::unique_ptr<expression> callee;
    token paren;
    std::vector<std::unique_ptr<expression>> arguments;

    call_expression(std::unique_ptr<expression> callee, token paren, std::vector<std::unique_ptr<expression>> arguments);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};


class get_expression : public expression
{
public:
    std::unique_ptr<expression> object;
    token name;

    get_expression(std::unique_ptr<expression> object_, token name_);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class set_expression : public expression
{
public:
    std::unique_ptr<expression> object;
    token name;
    std::unique_ptr<expression> value;

    set_expression(std::unique_ptr<expression> object_, token name_, std::unique_ptr<expression> value_);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class this_expression : public expression
{
public:
    token keyword;

    this_expression(const token& keyword_);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

class super_expression : public expression
{
public:
    token keyword;
    token method;

    super_expression(const token& keyword_, const token& method_);

    virtual std::string accept_visitor(expression_visitor<std::string>& v) override;
    virtual void accept_visitor(expression_visitor<void>& v) override;
    virtual literal_value accept_visitor(expression_visitor<literal_value>& v) override;
};

NAMESPACE_END

#endif
