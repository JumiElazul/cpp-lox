#ifndef JUMI_LUMINA_STATEMENTS_H
#define JUMI_LUMINA_STATEMENTS_H
#include "typedefs.h"
#include "expressions.h"
#include <vector>
#include <memory>

NAMESPACE_BEGIN(lumina)

class statement_visitor;

class statement
{
public:
    virtual ~statement() = default;
    virtual void accept_visitor(statement_visitor& v) = 0;
};

class variable_declaration_statement final : public statement
{
public:
    token ident_name;
    std::unique_ptr<expression> initializer_expr;

    variable_declaration_statement(const token& ident_name_, std::unique_ptr<expression> initializer_expr_);
    ~variable_declaration_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

class print_statement final : public statement
{
public:
    std::unique_ptr<expression> expr;

    print_statement(std::unique_ptr<expression> expr_);
    ~print_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

class if_statement final : public statement
{
public:
    std::unique_ptr<expression> condition;
    std::unique_ptr<statement> if_branch;
    std::unique_ptr<statement> else_branch;

    if_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> if_branch_, std::unique_ptr<statement> else_branch_);
    ~if_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

class while_statement final : public statement
{
public:
    std::unique_ptr<expression> condition;
    std::unique_ptr<statement> stmt_body;

    while_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> stmt_body_);
    ~while_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

class block_statement final : public statement
{
public:
    std::vector<std::unique_ptr<statement>> statements;

    block_statement(std::vector<std::unique_ptr<statement>>&& statements_);
    ~block_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

class expression_statement final : public statement
{
public:
    std::unique_ptr<expression> expr;

    expression_statement(std::unique_ptr<expression> expr_);
    ~expression_statement() = default;

    virtual void accept_visitor(statement_visitor& v) override;
};

NAMESPACE_END

#endif
