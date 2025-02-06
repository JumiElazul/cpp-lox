#ifndef JUMI_LUMINA_STATEMENT_VISITORS_H
#define JUMI_LUMINA_STATEMENT_VISITORS_H
#include "statements.h"
#include "typedefs.h"

NAMESPACE_BEGIN(lumina)

class print_statement;
class expression_statement;

class statement_visitor
{
public:
    virtual ~statement_visitor() = default;
    virtual void visit_print_statement(print_statement& stmt) = 0;
    virtual void visit_expression_statement(expression_statement& stmt) = 0;
    virtual void visit_variable_declaration_statement(variable_declaration_statement& stmt) = 0;
};

NAMESPACE_END

#endif
