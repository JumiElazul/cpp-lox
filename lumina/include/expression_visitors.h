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
class console_io;

template<typename T>
class expression_visitor
{
public:
    virtual ~expression_visitor() = default;
    virtual T visit_unary(const unary_expression& expr) const = 0;
    virtual T visit_binary(const binary_expression& expr) const = 0;
    virtual T visit_ternary(const ternary_expression& expr) const = 0;
    virtual T visit_literal(const literal_expression& expr) const = 0;
    virtual T visit_grouping(const grouping_expression& expr) const = 0;
};

class string_visitor final : public expression_visitor<std::string>
{
public:
    virtual std::string visit_unary(const unary_expression& expr) const override;
    virtual std::string visit_binary(const binary_expression& expr) const override;
    virtual std::string visit_ternary(const ternary_expression& expr) const override;
    virtual std::string visit_literal(const literal_expression& expr) const override;
    virtual std::string visit_grouping(const grouping_expression& expr) const override;
};

class tree_printer final : public expression_visitor<void>
{
public:
    tree_printer(console_io* io);

    virtual void visit_unary(const unary_expression& expr) const override;
    virtual void visit_binary(const binary_expression& expr) const override;
    virtual void visit_ternary(const ternary_expression& expr) const override;
    virtual void visit_literal(const literal_expression& expr) const override;
    virtual void visit_grouping(const grouping_expression& expr) const override;

private:
    console_io* _io;
};

NAMESPACE_END

#endif
