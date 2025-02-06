#ifndef JUMI_LUMINA_STATEMENTS_H
#define JUMI_LUMINA_STATEMENTS_H
#include "typedefs.h"
#include "expressions.h"

NAMESPACE_BEGIN(lumina)

class statement_visitor;

class statement
{
public:
    std::unique_ptr<expression> expr;

    statement(std::unique_ptr<expression> expr_);
    virtual ~statement() = default;

    virtual void accept_visitor(statement_visitor& v) = 0;
};

class expression_statement final : public statement
{
public:
    expression_statement(std::unique_ptr<expression> expr_);
    ~expression_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

class print_statement final : public statement
{
public:
    print_statement(std::unique_ptr<expression> expr_);
    ~print_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

class variable_declaration_statement final : public statement
{
public:
    token ident_name;

    variable_declaration_statement(const token& ident_name_, std::unique_ptr<expression> initializer_expr_);
    ~variable_declaration_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

NAMESPACE_END

#endif
