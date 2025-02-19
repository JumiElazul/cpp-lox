#ifndef JUMI_GEO_RESOLVER_H
#define JUMI_GEO_RESOLVER_H
#include "typedefs.h"
#include "expression_visitors.h"
#include "statement_visitors.h"
#include "statements.h"
#include <memory>
#include <vector>
#include <unordered_map>

NAMESPACE_BEGIN(geo)

class resolver : public statement_visitor, expression_visitor<void>
{
public:
    resolver();

    void resolve(const std::vector<std::unique_ptr<statement>>& statements);
    void resolve(const std::unique_ptr<statement>& stmt);
    void resolve(const std::unique_ptr<expression>& expr);

    virtual void visit_debug_statement(debug_statement& stmt) override;

    virtual void visit_function_declaration_statement(function_declaration_statement& stmt) override;
    virtual void visit_variable_declaration_statement(variable_declaration_statement& stmt) override;
    virtual void visit_print_statement(print_statement& stmt) override;
    virtual void visit_if_statement(if_statement& stmt) override;
    virtual void visit_while_statement(while_statement& stmt) override;
    virtual void visit_for_statement(for_statement& stmt) override;
    virtual void visit_break_statement(break_statement& stmt) override;
    virtual void visit_continue_statement(continue_statement& stmt) override;
    virtual void visit_return_statement(return_statement& stmt) override;
    virtual void visit_block_statement(block_statement& stmt) override;
    virtual void visit_expression_statement(expression_statement& stmt) override;

    virtual void visit_unary(unary_expression& expr) override;
    virtual void visit_binary(binary_expression& expr) override;
    virtual void visit_literal(literal_expression& expr) override;
    virtual void visit_grouping(grouping_expression& expr) override;
    virtual void visit_variable(variable_expression& expr) override;
    virtual void visit_assignment(assignment_expression& expr) override;
    virtual void visit_logical(logical_expression& expr) override;
    virtual void visit_postfix(postfix_expression& expr) override;
    virtual void visit_call(call_expression& expr) override;

private:
    std::vector<std::unordered_map<std::string, bool>> _scopes;

    void begin_scope();
    void end_scope();
};

NAMESPACE_END

#endif
