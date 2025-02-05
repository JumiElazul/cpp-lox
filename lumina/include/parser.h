#ifndef JUMI_LUMINA_PARSER_H
#define JUMI_LUMINA_PARSER_H
#include "typedefs.h"
#include "tokens.h"
#include "exceptions.h"
#include "statements.h"
#include <memory>
#include <optional>
#include <vector>

NAMESPACE_BEGIN(lumina)

class expression;
class console_io;
class parser
{
public:
    parser(const std::vector<token>& lexer_tokens, console_io* io);
    virtual ~parser() = default;
    virtual std::vector<std::unique_ptr<statement>> parse() = 0;

    bool error_occurred() const noexcept;
    void reset_error_flag() noexcept;

protected:
    const std::vector<token>& _lexer_tokens;
    uint32 _position;
    console_io* _io;
    bool _parser_error;
};

class recursive_descent_parser : public parser
{
public:
    recursive_descent_parser(const std::vector<token>& lexer_tokens, console_io* io);
    ~recursive_descent_parser() = default;

    virtual std::vector<std::unique_ptr<statement>> parse() override;

private:
    std::unique_ptr<statement> statement_precedence();
    std::unique_ptr<print_statement> create_print_statement();
    std::unique_ptr<expression_statement> create_expression_statement();

    std::unique_ptr<expression> expression_precedence();
    std::unique_ptr<expression> comma_precedence();
    std::unique_ptr<expression> ternary_precedence();
    std::unique_ptr<expression> equality_precedence();
    std::unique_ptr<expression> comparison_precedence();
    std::unique_ptr<expression> addition_precedence();
    std::unique_ptr<expression> multiplication_precedence();
    std::unique_ptr<expression> unary_precedence();
    std::unique_ptr<expression> primary_precedence();

    std::optional<token> advance_parser();
    std::optional<token> previous_token() const;
    std::optional<token> peek_next_token() const;
    void consume_if_matches(token_type type, const std::string& msg);
    bool check_type(token_type type);
    bool matches_token(const std::vector<token_type>& token_types);
    void validate_binary_has_lhs(const std::vector<token_type>& types);
    parser_error error(const std::string& msg, const token& t);
    void synchronize();
};

NAMESPACE_END

#endif
