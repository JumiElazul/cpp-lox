#ifndef JUMI_LUMINA_EXPRESSION_VISITORS_H
#define JUMI_LUMINA_EXPRESSION_VISITORS_H
#include "typedefs.h"
#include <string>

NAMESPACE_BEGIN(lumina)

class expression;
class binary_expression;
class unary_expression;

class expression_visitor
{
public:
    virtual ~expression_visitor() = default;
    virtual std::string visit_binary(const binary_expression& expr) const = 0;
    virtual std::string visit_unary(const unary_expression& expr) const = 0;
};

class string_visitor final : public expression_visitor
{
public:
    virtual std::string visit_binary(const binary_expression& expr) const override;
    virtual std::string visit_unary(const unary_expression& expr) const override;
};

NAMESPACE_END

#endif
