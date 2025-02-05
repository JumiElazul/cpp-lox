#ifndef JUMI_LUMINA_STATEMENTS_H
#define JUMI_LUMINA_STATEMENTS_H
#include "typedefs.h"
#include "expressions.h"
#include <memory>

NAMESPACE_BEGIN(lumina)

class statement_visitor;

class statement
{
public:
    std::unique_ptr<expression> expr;

    statement(std::unique_ptr<expression> expr_);
    virtual ~statement() = default;

    virtual void accept_visitor(const statement_visitor& v) const = 0;
};

class expression_statement final : public statement
{
public:
    expression_statement(std::unique_ptr<expression> expr_);
    ~expression_statement() = default;

    virtual void accept_visitor(const statement_visitor& v) const override;
};

class print_statement final : public statement
{
public:
    print_statement(std::unique_ptr<expression> expr_);
    ~print_statement() = default;

    virtual void accept_visitor(const statement_visitor& v) const override;
};

NAMESPACE_END

#endif
