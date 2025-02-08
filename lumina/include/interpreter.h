#ifndef JUMI_LUMINA_INTERPRETER_H
#define JUMI_LUMINA_INTERPRETER_H
#include "console_io.h"
#include "environment.h"
#include "exceptions.h"
#include "typedefs.h"
#include "tokens.h"
#include "expression_visitors.h"
#include "statements.h"
#include "statement_visitors.h"
#include "lumina_types.h"
#include <memory>
#include <vector>

NAMESPACE_BEGIN(lumina)

class environment;

class interpreter final : public statement_visitor, public expression_visitor<literal_value>
{
public:
    interpreter(console_io* io);
    ~interpreter();

    void interpret(const std::vector<std::unique_ptr<statement>>& statements, bool print_expr = false);

private:
    environment* _env;
    console_io* _io;
    bool _print_expr;

    virtual void visit_print_statement(print_statement& stmt) override;
    virtual void visit_block_statement(block_statement& stmt) override;
    virtual void visit_expression_statement(expression_statement& stmt) override;
    virtual void visit_variable_declaration_statement(variable_declaration_statement& stmt) override;

    virtual literal_value visit_unary(unary_expression& expr) override;
    virtual literal_value visit_binary(binary_expression& expr) override;
    virtual literal_value visit_ternary(ternary_expression& expr) override;
    virtual literal_value visit_literal(literal_expression& expr) override;
    virtual literal_value visit_grouping(grouping_expression& expr) override;
    virtual literal_value visit_variable(variable_expression& expr) override;
    virtual literal_value visit_assignment(assignment_expression& expr) override;

    bool is_truthy(const literal_value& literal) const;
    bool is_equal(const literal_value& lhs, const literal_value& rhs) const;

    lumina_type_error type_error(const std::string& msg, const token& t) const;
};

NAMESPACE_END

#endif
