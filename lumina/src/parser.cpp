#include "parser.h"
#include "expressions.h"
#include "typedefs.h"
#include <iostream>
#include <vector>

NAMESPACE_BEGIN(lumina)

recursive_descent_parser::recursive_descent_parser(const std::vector<token>& lexer_tokens)
    : _lexer_tokens(lexer_tokens)
    , _position()
{

}

void recursive_descent_parser::parse()
{

}

bool recursive_descent_parser::match(const std::vector<token_type>& token_types)
{
    for (token_type type : token_types)
    {
        if (_lexer_tokens[_position].type == type)
        {
            return true;
        }
    }
    return false;
}

std::unique_ptr<expression> recursive_descent_parser::expression_precedence()
{
    std::unique_ptr<expression> expr = equality_precedence();

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::equality_precedence()
{
    std::unique_ptr<expression> expr = comparison_precedence();

    while (match({ token_type::bang_equal_, token_type::equal_equal_ }))
    {
        token oper = previous();
        std::unique_ptr<expression> rhs = comparison_precedence();
        expr = std::make_unique<binary_expression>(expr, oper, rhs);
    }

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::comparison_precedence()
{
    std::unique_ptr<expression> expr = term_precedence();

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::term_precedence()
{
    std::unique_ptr<expression> expr = factor_precedence();

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::factor_precedence()
{
    std::unique_ptr<expression> expr = unary_precedence();

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::unary_precedence()
{
    std::unique_ptr<expression> expr = primary_precedence();

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::primary_precedence()
{
    return nullptr;
}

NAMESPACE_END
