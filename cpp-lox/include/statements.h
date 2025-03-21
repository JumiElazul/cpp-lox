#ifndef JUMI_CPPLOX_STATEMENTS_H
#define JUMI_CPPLOX_STATEMENTS_H
#include "typedefs.h"
#include "expressions.h"
#include "tokens.h"
#include <vector>
#include <memory>

NAMESPACE_BEGIN(cpplox)

class statement_visitor;
class block_statement;

class statement
{
public:
    virtual ~statement() = default;
    virtual void accept_visitor(statement_visitor& v) = 0;

protected:
    statement() = default;
    statement(const statement&) = delete;
    statement& operator=(const statement&) = delete;
    statement(statement&&) = default;
    statement& operator=(statement&&) = default;
};

// --------------------------------------------------
// DEBUG STATEMENT
class debug_statement final : public statement
{
public:
    debug_statement();

    virtual void accept_visitor(statement_visitor& v) override;
};
// DEBUG STATEMENT
// --------------------------------------------------

class function_declaration_statement final : public statement
{
public:
    token ident_name;
    std::vector<token> params;
    std::vector<std::unique_ptr<statement>> body;
    bool static_method;

    function_declaration_statement(const token& ident_name_, const std::vector<token>& params_, std::vector<std::unique_ptr<statement>>&& body_, bool static_method_ = false);

    virtual void accept_visitor(statement_visitor& v) override;
};

class variable_declaration_statement final : public statement
{
public:
    token ident_name;
    std::unique_ptr<expression> initializer_expr;

    variable_declaration_statement(const token& ident_name_, std::unique_ptr<expression> initializer_expr_);

    virtual void accept_visitor(statement_visitor& v) override;
};

class if_statement final : public statement
{
public:
    std::unique_ptr<expression> condition;
    std::unique_ptr<statement> if_branch;
    std::unique_ptr<statement> else_branch;

    if_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> if_branch_, std::unique_ptr<statement> else_branch_);

    virtual void accept_visitor(statement_visitor& v) override;
};

class while_statement final : public statement
{
public:
    std::unique_ptr<expression> condition;
    std::unique_ptr<statement> stmt_body;

    while_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> stmt_body_);

    virtual void accept_visitor(statement_visitor& v) override;
};

class for_statement final : public statement
{
public:
    std::unique_ptr<statement> initializer;
    std::unique_ptr<expression> condition;
    std::unique_ptr<expression> increment;
    std::unique_ptr<statement> stmt_body;
    token for_token;

    for_statement(std::unique_ptr<statement> initializer_, std::unique_ptr<expression> condition_,
            std::unique_ptr<expression> increment_, std::unique_ptr<statement>&& stmt_body_, const token& for_token_);

    virtual void accept_visitor(statement_visitor& v) override;
};

class break_statement final : public statement
{
public:
    token break_token;

    break_statement(const token& t);

    virtual void accept_visitor(statement_visitor& v) override;
};

class continue_statement final : public statement
{
public:
    token continue_token;

    continue_statement(const token& t);

    virtual void accept_visitor(statement_visitor& v) override;
};

class return_statement final : public statement
{
public:
    token keyword;
    std::unique_ptr<expression> return_expr;

    return_statement(const token& keyword_, std::unique_ptr<expression> return_expr_);

    virtual void accept_visitor(statement_visitor& v) override;
};

class block_statement final : public statement
{
public:
    std::vector<std::unique_ptr<statement>> statements;

    block_statement(std::vector<std::unique_ptr<statement>>&& statements_);

    virtual void accept_visitor(statement_visitor& v) override;
};

class class_statement final : public statement
{
public:
    token name;
    std::vector<std::unique_ptr<function_declaration_statement>> methods;
    std::unique_ptr<expression> superclass;

    class_statement(const token& name_, std::vector<std::unique_ptr<function_declaration_statement>>&& methods_,
            std::unique_ptr<expression> superclass_);

    virtual void accept_visitor(statement_visitor& v) override;
};

class expression_statement final : public statement
{
public:
    std::unique_ptr<expression> expr;

    expression_statement(std::unique_ptr<expression> expr_);

    virtual void accept_visitor(statement_visitor& v) override;
};

NAMESPACE_END

#endif
