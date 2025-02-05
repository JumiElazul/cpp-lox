#include "parser.h"
#include "console_io.h"
#include "exceptions.h"
#include "expressions.h"
#include "tokens.h"
#include "typedefs.h"
#include <limits>
#include <optional>
#include <vector>
#include <memory>

NAMESPACE_BEGIN(lumina)

parser::parser(const std::vector<token>& lexer_tokens, console_io* io)
    : _lexer_tokens(lexer_tokens)
    , _position(0)
    , _io(io)
    , _parser_error(false)
{

}

bool parser::error_occurred() const noexcept { return _parser_error; }
void parser::reset_error_flag() noexcept { _parser_error = false; }

recursive_descent_parser::recursive_descent_parser(const std::vector<token>& lexer_tokens, console_io* io)
    : parser(lexer_tokens, io)
{

}

std::unique_ptr<expression> recursive_descent_parser::parse()
{
    try
    {
        // Ignore the beginning of file token for now, until we decide if we want to use it for something
        if (!_lexer_tokens.empty() && _lexer_tokens.front().type == token_type::bof_)
            _position = 1;

        return expression_precedence();
    }
    catch (const parser_error& e)
    {
        _parser_error = true;
        _io->err() << e.what() << '\n';
        return nullptr;
    }
}

std::optional<token> recursive_descent_parser::advance_parser()
{
    const std::optional<token>& token = peek_next_token();
    if (token.has_value() && token->type != token_type::eof_)
        ++_position;

    return previous_token();
}

std::optional<token> recursive_descent_parser::previous_token() const
{
    uint32 index = _position - 1;
    if (index == std::numeric_limits<uint32>::max())
        return std::nullopt;

    return _lexer_tokens[index];
}

std::optional<token> recursive_descent_parser::peek_next_token() const
{
    if (_position >= _lexer_tokens.size())
        return std::nullopt;

    return _lexer_tokens[_position];
}

void recursive_descent_parser::consume_if_matches(token_type type, const std::string& msg)
{
    if (check_type(type))
    {
        advance_parser();
    }
    else
    {
        throw error(msg, *peek_next_token());
    }
}

bool recursive_descent_parser::check_type(token_type type)
{
    if (!peek_next_token().has_value())
        return false;

    return peek_next_token()->type == type;
}

bool recursive_descent_parser::matches_token(const std::vector<token_type>& token_types)
{
    for (token_type type : token_types)
    {
        if (check_type(type))
        {
            advance_parser();
            return true;
        }
    }
    return false;
}

std::unique_ptr<expression> recursive_descent_parser::expression_precedence()
{
    // expression -> comma;
    return comma_precedence();
}

std::unique_ptr<expression> recursive_descent_parser::comma_precedence()
{
    // comma      -> ternary ( "," ternary )*;
    validate_binary_has_lhs({ token_type::comma_ });

    std::unique_ptr<expression> expr = ternary_precedence();

    while (matches_token({ token_type::comma_ }))
    {
        token oper = *previous_token();
        std::unique_ptr<expression> rhs = ternary_precedence();
        expr = std::make_unique<binary_expression>(std::move(expr), oper, std::move(rhs));
    }

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::ternary_precedence()
{
    // ternary    -> equality ( "?" equality )?
    validate_binary_has_lhs({ token_type::question_ });

    std::unique_ptr<expression> expr = equality_precedence();

    if (matches_token({ token_type::question_ }))
    {
        token oper = *previous_token();
        std::unique_ptr<expression> then_expr = expression_precedence();
        consume_if_matches(token_type::colon_, "Expected ':' after ternary expression.");
        std::unique_ptr<expression> else_expr = expression_precedence();
        expr = std::make_unique<ternary_expression>(std::move(expr), oper, std::move(then_expr), std::move(else_expr));
    }

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::equality_precedence()
{
    // equality   -> comparison ( ( "!=" | "\==" ) comparison)\*;
    validate_binary_has_lhs({ token_type::bang_equal_, token_type::equal_equal_ });

    std::unique_ptr<expression> expr = comparison_precedence();

    while (matches_token({ token_type::bang_equal_, token_type::equal_equal_ }))
    {
        token oper = *previous_token();
        std::unique_ptr<expression> rhs = comparison_precedence();
        expr = std::make_unique<binary_expression>(std::move(expr), oper, std::move(rhs));
    }

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::comparison_precedence()
{
    // comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )\*;
    validate_binary_has_lhs({ token_type::greater_, token_type::greater_equal_, token_type::less_, token_type::less_equal_ });

    std::unique_ptr<expression> expr = addition_precedence();

    while (matches_token({ token_type::greater_, token_type::greater_equal_, token_type::less_, token_type::less_equal_ }))
    {
        token oper = *previous_token();
        std::unique_ptr<expression> rhs = addition_precedence();
        expr = std::make_unique<binary_expression>(std::move(expr), oper, std::move(rhs));
    }

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::addition_precedence()
{
    // term       -> factor ( ( "-" | "+" ) factor )\*;
    validate_binary_has_lhs({ token_type::plus_ });

    std::unique_ptr<expression> expr = multiplication_precedence();

    while (matches_token({ token_type::minus_, token_type::plus_ }))
    {
        token oper = *previous_token();
        std::unique_ptr<expression> rhs = multiplication_precedence();
        expr = std::make_unique<binary_expression>(std::move(expr), oper, std::move(rhs));
    }

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::multiplication_precedence()
{
    // factor     -> unary ( ( "\*" | "/" ) unary )\*;
    validate_binary_has_lhs({ token_type::star_, token_type::slash_ });

    std::unique_ptr<expression> expr = unary_precedence();

    while (matches_token({ token_type::star_, token_type::slash_ }))
    {
        token oper = *previous_token();
        std::unique_ptr<expression> rhs = unary_precedence();
        expr = std::make_unique<binary_expression>(std::move(expr), oper, std::move(rhs));
    }

    return expr;
}

std::unique_ptr<expression> recursive_descent_parser::unary_precedence()
{
    // unary      -> ( "!" | "-" ) unary | primary;
    if (matches_token({ token_type::bang_, token_type::minus_ }))
    {
        token oper = *previous_token();
        std::unique_ptr<expression> rhs = unary_precedence();
        return std::make_unique<unary_expression>(oper, std::move(rhs));
    }

    return primary_precedence();
}

std::unique_ptr<expression> recursive_descent_parser::primary_precedence()
{
    // primary    -> NUMBER | STRING | "true" | "false" | "null" | "(" expression ")";
    if (matches_token({ token_type::false_ })) return std::make_unique<literal_expression>(false);
    if (matches_token({ token_type::true_ }))  return std::make_unique<literal_expression>(true);
    if (matches_token({ token_type::null_ }))  return std::make_unique<literal_expression>(std::monostate{});

    if (matches_token({ token_type::number_, token_type::string_ }))
        return std::make_unique<literal_expression>(previous_token()->literal);

    if (matches_token({ token_type::left_paren_ }))
    {
        std::unique_ptr<expression> expr = expression_precedence();
        consume_if_matches(token_type::right_paren_, "Expected ')' after expression.");
        return std::make_unique<grouping_expression>(std::move(expr));
    }

    throw error("Expected expression.", *previous_token());
}

void recursive_descent_parser::validate_binary_has_lhs(const std::vector<token_type>& types)
{
    if (matches_token(types))
    {
        token oper = *previous_token();
        throw error("Missing left-hand operand for '" + oper.lexeme + "' operator.", oper);
        _parser_error = true;
    }
}

parser_error recursive_descent_parser::error(const std::string& msg, const token& t)
{
    return parser_error(msg, t);
}

void recursive_descent_parser::synchronize()
{
    advance_parser();

    while (peek_next_token().has_value())
    {
        if (previous_token()->type == token_type::semicolon_)
            return;

        switch (peek_next_token()->type)
        {
            case token_type::class_:
            case token_type::func_:
            case token_type::var_:
            case token_type::for_:
            case token_type::if_:
            case token_type::while_:
            case token_type::print_:
            case token_type::return_:
                return;
            default:
                break;
        }

        advance_parser();
    }
}

NAMESPACE_END
