#ifndef JUMI_GEO_INTERPRETER_H
#define JUMI_GEO_INTERPRETER_H
#include "console_io.h"
#include "environment.h"
#include "exceptions.h"
#include "typedefs.h"
#include "tokens.h"
#include "expression_visitors.h"
#include "statements.h"
#include "statement_visitors.h"
#include "geo_types.h"
#include <memory>
#include <vector>

NAMESPACE_BEGIN(geo)

class environment;

class interpreter final : public statement_visitor, public expression_visitor<literal_value>
{
    friend class geo_function;

    struct geo_loop_break { };
    struct geo_loop_continue { };

    class environment_scope_guard
    {
    public:
        environment_scope_guard(environment*& interpreter_curr_env, std::unique_ptr<environment> new_env = nullptr);
        ~environment_scope_guard();

    private:
        environment*& _interpreter_curr_env;
        std::unique_ptr<environment> _new_env;
        environment* _prev_env;
    };

public:
    interpreter(console_io* io);

    void interpret(const std::vector<std::unique_ptr<statement>>& statements);
    [[nodiscard]] environment* global_environment() const;
    [[nodiscard]] environment* current_environment() const;

private:
    std::unique_ptr<environment> _globals;
    environment* _curr_env;
    console_io* _io;

    literal_value evaluate(const std::unique_ptr<expression>& expr);
    void evaluate(const std::unique_ptr<statement>& stmt);
    void execute_block(const std::vector<std::unique_ptr<statement>>& statements, std::unique_ptr<environment> new_environment = nullptr);

    virtual void visit_function_declaration_statement(function_declaration_statement& stmt) override;
    virtual void visit_variable_declaration_statement(variable_declaration_statement& stmt) override;
    virtual void visit_print_statement(print_statement& stmt) override;
    virtual void visit_if_statement(if_statement& stmt) override;
    virtual void visit_while_statement(while_statement& stmt) override;
    virtual void visit_for_statement(for_statement& stmt) override;
    virtual void visit_break_statement(break_statement& stmt) override;
    virtual void visit_continue_statement(continue_statement& stmt) override;
    virtual void visit_block_statement(block_statement& stmt) override;
    virtual void visit_expression_statement(expression_statement& stmt) override;

    virtual literal_value visit_unary(unary_expression& expr) override;
    virtual literal_value visit_binary(binary_expression& expr) override;
    virtual literal_value visit_ternary(ternary_expression& expr) override;
    virtual literal_value visit_literal(literal_expression& expr) override;
    virtual literal_value visit_grouping(grouping_expression& expr) override;
    virtual literal_value visit_variable(variable_expression& expr) override;
    virtual literal_value visit_assignment(assignment_expression& expr) override;
    virtual literal_value visit_logical(logical_expression& expr) override;
    virtual literal_value visit_postfix(postfix_expression& expr) override;
    virtual literal_value visit_prefix(prefix_expression& expr) override;
    virtual literal_value visit_call(call_expression& expr) override;

    bool is_truthy(const literal_value& literal) const;
    bool is_equal(const literal_value& lhs, const literal_value& rhs) const;

    geo_type_error type_error(const std::string& msg, const token& t) const;
};

NAMESPACE_END

#endif
