#ifndef JUMI_LUMINA_INTERPRETER_H
#define JUMI_LUMINA_INTERPRETER_H
#include "typedefs.h"
#include "expression_visitors.h"
#include "lumina_types.h"

NAMESPACE_BEGIN(lumina)

class interpreter final : public expression_visitor<literal_value>
{
public:
    virtual literal_value visit_unary(const unary_expression& expr) const override;
    virtual literal_value visit_binary(const binary_expression& expr) const override;
    virtual literal_value visit_ternary(const ternary_expression& expr) const override;
    virtual literal_value visit_literal(const literal_expression& expr) const override;
    virtual literal_value visit_grouping(const grouping_expression& expr) const override;

    literal_value evaluate(const expression& expr) const;
};

NAMESPACE_END

#endif
