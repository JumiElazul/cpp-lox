#ifndef JUMI_LUMINA_EXPRESSION_VISITORS_H
#define JUMI_LUMINA_EXPRESSION_VISITORS_H
#include "typedefs.h"
#include <string>

NAMESPACE_BEGIN(lumina)

class expression;
class unary_expression;
class binary_expression;
class ternary_expression;
class literal_expression;
class grouping_expression;
class variable_expression;
class assignment_expression;
class logical_expression;
class postfix_expression;
class prefix_expression;
class console_io;

template<typename T>
class expression_visitor
{
public:
    virtual ~expression_visitor() = default;
    virtual T visit_unary(unary_expression& expr) = 0;
    virtual T visit_binary(binary_expression& expr) = 0;
    virtual T visit_ternary(ternary_expression& expr) = 0;
    virtual T visit_literal(literal_expression& expr) = 0;
    virtual T visit_grouping(grouping_expression& expr) = 0;
    virtual T visit_variable(variable_expression& expr) = 0;
    virtual T visit_assignment(assignment_expression& expr) = 0;
    virtual T visit_logical(logical_expression& expr) = 0;
    virtual T visit_postfix(postfix_expression& expr) = 0;
    virtual T visit_prefix(prefix_expression& expr) = 0;
};

class string_visitor final : public expression_visitor<std::string>
{
public:
    virtual std::string visit_unary(unary_expression& expr) override;
    virtual std::string visit_binary(binary_expression& expr) override;
    virtual std::string visit_ternary(ternary_expression& expr) override;
    virtual std::string visit_literal(literal_expression& expr) override;
    virtual std::string visit_grouping(grouping_expression& expr) override;
    virtual std::string visit_variable(variable_expression& expr) override;
    virtual std::string visit_assignment(assignment_expression& expr) override;
    virtual std::string visit_logical(logical_expression& expr) override;
    virtual std::string visit_postfix(postfix_expression& expr) override;
    virtual std::string visit_prefix(prefix_expression& expr) override;
};

NAMESPACE_END

#endif
