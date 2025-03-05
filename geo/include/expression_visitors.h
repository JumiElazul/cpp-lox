#ifndef JUMI_GEO_EXPRESSION_VISITORS_H
#define JUMI_GEO_EXPRESSION_VISITORS_H
#include "typedefs.h"

NAMESPACE_BEGIN(geo)

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
class call_expression;
class get_expression;
class set_expression;
class this_expression;
class console_io;

template<typename T>
class expression_visitor
{
public:
    virtual ~expression_visitor() = default;
    virtual T visit_unary(unary_expression& expr) = 0;
    virtual T visit_binary(binary_expression& expr) = 0;
    virtual T visit_literal(literal_expression& expr) = 0;
    virtual T visit_grouping(grouping_expression& expr) = 0;
    virtual T visit_variable(variable_expression& expr) = 0;
    virtual T visit_assignment(assignment_expression& expr) = 0;
    virtual T visit_logical(logical_expression& expr) = 0;
    virtual T visit_postfix(postfix_expression& expr) = 0;
    virtual T visit_call(call_expression& expr) = 0;
    virtual T visit_get(get_expression& expr) = 0;
    virtual T visit_set(set_expression& expr) = 0;
    virtual T visit_this(this_expression& expr) = 0;
};

NAMESPACE_END

#endif
