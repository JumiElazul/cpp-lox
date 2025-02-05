#include "statements.h"
#include "statement_visitors.h"
#include "expressions.h"
#include "typedefs.h"
#include <memory>

NAMESPACE_BEGIN(lumina)

statement::statement(std::unique_ptr<expression> expr_)
    : expr(std::move(expr_))
{
}

expression_statement::expression_statement(std::unique_ptr<expression> expr_)
    : statement(std::move(expr_))
{
}

void expression_statement::accept_visitor(const statement_visitor& v) const
{
    v.visit_expression_statement(*this);
}

print_statement::print_statement(std::unique_ptr<expression> expr_)
    : statement(std::move(expr_))
{
}

void print_statement::accept_visitor(const statement_visitor& v) const
{
    v.visit_print_statement(*this);
}

NAMESPACE_END
