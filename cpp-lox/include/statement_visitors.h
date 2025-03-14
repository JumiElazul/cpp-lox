#ifndef JUMI_CPPLOX_STATEMENT_VISITORS_H
#define JUMI_CPPLOX_STATEMENT_VISITORS_H
#include "statements.h"
#include "typedefs.h"

NAMESPACE_BEGIN(cpplox)

class environment;

class statement_visitor
{
public:
    virtual ~statement_visitor() = default;

    virtual void visit_debug_statement(debug_statement& stmt) = 0;

    virtual void visit_function_declaration_statement(function_declaration_statement& stmt) = 0;
    virtual void visit_variable_declaration_statement(variable_declaration_statement& stmt) = 0;
    virtual void visit_if_statement(if_statement& stmt) = 0;
    virtual void visit_while_statement(while_statement& stmt) = 0;
    virtual void visit_for_statement(for_statement& stmt) = 0;
    virtual void visit_break_statement(break_statement& stmt) = 0;
    virtual void visit_continue_statement(continue_statement& stmt) = 0;
    virtual void visit_return_statement(return_statement& stmt) = 0;
    virtual void visit_block_statement(block_statement& stmt) = 0;
    virtual void visit_class_statement(class_statement& stmt) = 0;
    virtual void visit_expression_statement(expression_statement& stmt) = 0;
};

NAMESPACE_END

#endif
