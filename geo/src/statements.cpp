#include "statements.h"
#include "statement_visitors.h"
#include "expressions.h"
#include "typedefs.h"
#include <memory>
#include <vector>

NAMESPACE_BEGIN(geo)

// --------------------------------------------------
// DEBUG STATEMENT

debug_statement::debug_statement() { }
void debug_statement::accept_visitor(statement_visitor& v) { v.visit_debug_statement(*this); }

// DEBUG STATEMENT
// --------------------------------------------------

function_declaration_statement::function_declaration_statement(const token& ident_name_, const std::vector<token>& params_, std::unique_ptr<statement>&& body_)
    : ident_name(ident_name_)
    , params(params_)
    , body(std::move(body_)) { }

variable_declaration_statement::variable_declaration_statement(const token& ident_name_, std::unique_ptr<expression> initializer_expr_)
    : ident_name(ident_name_)
    , initializer_expr(std::move(initializer_expr_)) { }

print_statement::print_statement(std::unique_ptr<expression> expr_)
    : expr(std::move(expr_)) { }

if_statement::if_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> if_branch_, std::unique_ptr<statement> else_branch_)
    : condition(std::move(condition_))
    , if_branch(std::move(if_branch_))
    , else_branch(std::move(else_branch_)) { }

while_statement::while_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> stmt_body_)
    : condition(std::move(condition_))
    , stmt_body(std::move(stmt_body_)) { }

for_statement::for_statement(std::unique_ptr<statement> initializer_, std::unique_ptr<expression> condition_,
        std::unique_ptr<expression> increment_, std::unique_ptr<statement> stmt_body_)
    : initializer(std::move(initializer_))
    , condition(std::move(condition_))
    , increment(std::move(increment_))
    , stmt_body(std::move(stmt_body_)) { }

break_statement::break_statement(const token& t)
    : break_token(t) { }

continue_statement::continue_statement(const token& t)
    : continue_token(t) { }

return_statement::return_statement(const token& keyword_, std::unique_ptr<expression> expr_)
    : keyword(keyword_)
    , return_expr(std::move(expr_)) { }

block_statement::block_statement(std::vector<std::unique_ptr<statement>>&& statements_)
    : statements(std::move(statements_)) { }

expression_statement::expression_statement(std::unique_ptr<expression> expr_)
    : expr(std::move(expr_)) { }

void function_declaration_statement::accept_visitor(statement_visitor& v) { v.visit_function_declaration_statement(*this); }
void variable_declaration_statement::accept_visitor(statement_visitor& v) { v.visit_variable_declaration_statement(*this); }
void print_statement::accept_visitor(statement_visitor& v)                { v.visit_print_statement(*this); }
void if_statement::accept_visitor(statement_visitor& v)                   { v.visit_if_statement(*this); }
void while_statement::accept_visitor(statement_visitor& v)                { v.visit_while_statement(*this); }
void for_statement::accept_visitor(statement_visitor& v)                  { v.visit_for_statement(*this); }
void break_statement::accept_visitor(statement_visitor& v)                { v.visit_break_statement(*this); }
void continue_statement::accept_visitor(statement_visitor& v)             { v.visit_continue_statement(*this); }
void return_statement::accept_visitor(statement_visitor& v)               { v.visit_return_statement(*this); }
void block_statement::accept_visitor(statement_visitor& v)                { v.visit_block_statement(*this); }
void expression_statement::accept_visitor(statement_visitor& v)           { v.visit_expression_statement(*this); }

NAMESPACE_END
