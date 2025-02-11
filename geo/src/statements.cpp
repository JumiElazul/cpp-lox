#include "statements.h"
#include "statement_visitors.h"
#include "expressions.h"
#include "typedefs.h"
#include <memory>
#include <vector>

NAMESPACE_BEGIN(geo)

variable_declaration_statement::variable_declaration_statement(const token& ident_name_, std::unique_ptr<expression> initializer_expr_)
    : ident_name(ident_name_)
    , initializer_expr(std::move(initializer_expr_)) { }

void variable_declaration_statement::accept_visitor(statement_visitor& v)
{
    v.visit_variable_declaration_statement(*this);
}

print_statement::print_statement(std::unique_ptr<expression> expr_)
    : expr(std::move(expr_)) { }

void print_statement::accept_visitor(statement_visitor& v)
{
    v.visit_print_statement(*this);
}

if_statement::if_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> if_branch_, std::unique_ptr<statement> else_branch_)
    : condition(std::move(condition_))
    , if_branch(std::move(if_branch_))
    , else_branch(std::move(else_branch_))
{

}

void if_statement::accept_visitor(statement_visitor& v)
{
    v.visit_if_statement(*this);
}

while_statement::while_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> stmt_body_)
    : condition(std::move(condition_))
    , stmt_body(std::move(stmt_body_))
{

}

void while_statement::accept_visitor(statement_visitor& v)
{
    v.visit_while_statement(*this);
}

for_statement::for_statement(std::unique_ptr<statement> initializer_, std::unique_ptr<expression> condition_,
        std::unique_ptr<expression> increment_, std::unique_ptr<statement> stmt_body_)
    : initializer(std::move(initializer_))
    , condition(std::move(condition_))
    , increment(std::move(increment_))
    , stmt_body(std::move(stmt_body_))
{

}

void for_statement::accept_visitor(statement_visitor& v)
{
    v.visit_for_statement(*this);
}

break_statement::break_statement(const token& t)
    : break_token(t)
{ }

void break_statement::accept_visitor(statement_visitor& v)
{
    v.visit_break_statement(*this);
}

continue_statement::continue_statement(const token& t)
    : continue_token(t)
{ }

void continue_statement::accept_visitor(statement_visitor& v)
{
    v.visit_continue_statement(*this);
}

block_statement::block_statement(std::vector<std::unique_ptr<statement>>&& statements_)
    : statements(std::move(statements_))
{ }

void block_statement::accept_visitor(statement_visitor& v)
{
    v.visit_block_statement(*this);
}

expression_statement::expression_statement(std::unique_ptr<expression> expr_)
    : expr(std::move(expr_)) { }

void expression_statement::accept_visitor(statement_visitor& v)
{
    v.visit_expression_statement(*this);
}


NAMESPACE_END
