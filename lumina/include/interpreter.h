#ifndef JUMI_LUMINA_INTERPRETER_H
#define JUMI_LUMINA_INTERPRETER_H
#include "console_io.h"
#include "typedefs.h"
#include "tokens.h"
#include "expression_visitors.h"
#include "statements.h"
#include "statement_visitors.h"
#include "lumina_types.h"
#include <memory>
#include <vector>

NAMESPACE_BEGIN(lumina)

class interpreter final : public statement_visitor, public expression_visitor<literal_value>
{
public:
    interpreter(console_io* io);

    void interpret(const std::vector<std::unique_ptr<statement>>& statements);

private:
    console_io* _io;

    virtual void visit_print_statement(const print_statement& stmt) const override;
    virtual void visit_expression_statement(const expression_statement& stmt) const override;

    virtual literal_value visit_unary(const unary_expression& expr) const override;
    virtual literal_value visit_binary(const binary_expression& expr) const override;
    virtual literal_value visit_ternary(const ternary_expression& expr) const override;
    virtual literal_value visit_literal(const literal_expression& expr) const override;
    virtual literal_value visit_grouping(const grouping_expression& expr) const override;

    literal_value handle_unary(const token& oper, const literal_value& literal) const;
    literal_value handle_binary(const literal_value& lhs, const token& oper, const literal_value& rhs) const;
    literal_value handle_ternary(const literal_value& if_literal, const token& oper, const literal_value& then_literal, const literal_value& else_literal) const;

    bool is_truthy(const literal_value& literal) const;
    bool is_equal(const literal_value& lhs, const literal_value& rhs) const;
};

NAMESPACE_END

#endif
