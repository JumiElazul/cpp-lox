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
    virtual std::string accept_visitor(const expression_visitor<std::string>& v) const = 0;
};

class unary_expression : public expression
{
public:
    token oper;
    std::unique_ptr<expression> expr_rhs;

    unary_expression(token oper, std::unique_ptr<expression> expr);

    std::string accept_visitor(const expression_visitor<std::string>& v) const override;
};

class binary_expression : public expression
{
public:
    std::unique_ptr<expression> expr_lhs;
    token oper;
    std::unique_ptr<expression> expr_rhs;

    binary_expression(std::unique_ptr<expression> lhs, token oper, std::unique_ptr<expression> rhs);

    std::string accept_visitor(const expression_visitor<std::string>& v) const override;
};

class ternary_expression : public expression
{
public:
    std::unique_ptr<expression> expr_lhs;
    token oper;
    std::unique_ptr<expression> expr_then;
    std::unique_ptr<expression> expr_else;

    ternary_expression(std::unique_ptr<expression> lhs, token oper, std::unique_ptr<expression> expr_then, std::unique_ptr<expression> expr_else);

    std::string accept_visitor(const expression_visitor<std::string>& v) const override;
};

class literal_expression : public expression
{
public:
    literal_value literal_val;

    literal_expression(const literal_value& literal);

    std::string accept_visitor(const expression_visitor<std::string>& v) const override;
};

class grouping_expression : public expression
{
public:
    std::unique_ptr<expression> expr_lhs;

    grouping_expression(std::unique_ptr<expression> expr);

    std::string accept_visitor(const expression_visitor<std::string>& v) const override;
};

NAMESPACE_END

#endif
