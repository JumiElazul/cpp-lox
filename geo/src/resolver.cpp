#include "resolver.h"
#include "typedefs.h"
#include "statements.h"
#include <memory>
#include <vector>
#include <unordered_map>

NAMESPACE_BEGIN(geo)

resolver::resolver()
    : _scopes() { }

void resolver::resolve(const std::vector<std::unique_ptr<statement>>& statements)
{
    for (const auto& stmt : statements)
    {
        resolve(stmt);
    }
}

void resolver::resolve(const std::unique_ptr<statement>& stmt)
{
    stmt->accept_visitor(*this);
}

void resolver::resolve(const std::unique_ptr<expression>& expr)
{
    expr->accept_visitor(*this);
}

void resolver::visit_debug_statement(debug_statement& stmt)
{

}

void resolver::visit_function_declaration_statement(function_declaration_statement& stmt)
{

}

void resolver::visit_variable_declaration_statement(variable_declaration_statement& stmt)
{

}

void resolver::visit_print_statement(print_statement& stmt)
{

}

void resolver::visit_if_statement(if_statement& stmt)
{

}

void resolver::visit_while_statement(while_statement& stmt)
{

}

void resolver::visit_for_statement(for_statement& stmt)
{

}

void resolver::visit_break_statement(break_statement& stmt)
{

}

void resolver::visit_continue_statement(continue_statement& stmt)
{

}

void resolver::visit_return_statement(return_statement& stmt)
{

}

void resolver::visit_block_statement(block_statement& stmt)
{
    begin_scope();
    resolve(stmt.statements);
    end_scope();
}

void resolver::visit_expression_statement(expression_statement& stmt)
{

}

void resolver::visit_unary(unary_expression& expr)
{

}

void resolver::visit_binary(binary_expression& expr)
{

}

void resolver::visit_literal(literal_expression& expr)
{

}

void resolver::visit_grouping(grouping_expression& expr)
{

}

void resolver::visit_variable(variable_expression& expr)
{

}

void resolver::visit_assignment(assignment_expression& expr)
{

}

void resolver::visit_logical(logical_expression& expr)
{

}

void resolver::visit_postfix(postfix_expression& expr)
{

}

void resolver::visit_call(call_expression& expr)
{

}

void resolver::begin_scope()
{
    _scopes.push_back(std::unordered_map<std::string, bool>());
}

void resolver::end_scope()
{

}

NAMESPACE_END

