#ifndef JUMI_LUMINA_EXPRESSIONS_H
#define JUMI_LUMINA_EXPRESSIONS_H
#include "typedefs.h"
#include "tokens.h"
#include <memory>
#include <string>

NAMESPACE_BEGIN(lumina)

class expression_visitor;
class expression
{
public:
    virtual ~expression() = default;
    virtual std::string accept_visitor(const expression_visitor& v) const = 0;
};

class binary_expression : public expression
{
public:
    std::unique_ptr<expression> expr_lhs;
    token oper;
    std::unique_ptr<expression> expr_rhs;

    std::string accept_visitor(const expression_visitor& v) const override;
};

class unary_expression : public expression
{
public:
    token oper;
    std::unique_ptr<expression> expr;

    std::string accept_visitor(const expression_visitor& v) const override;
};

NAMESPACE_END

#endif
