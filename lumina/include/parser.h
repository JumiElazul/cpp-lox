#ifndef JUMI_LUMINA_PARSER_H
#define JUMI_LUMINA_PARSER_H
#include "typedefs.h"
#include "tokens.h"
#include "parser_exceptions.h"
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
    virtual std::unique_ptr<expression> parse() = 0;

    bool error_occurred() const noexcept;

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

    virtual std::unique_ptr<expression> parse() override;

private:
    std::optional<token> advance_parser();
    std::optional<token> previous_token() const;
    std::optional<token> peek_next_token() const;
    void consume_if_matches(token_type type, const std::string& msg);
    bool check_type(token_type type);
    bool matches_token(const std::vector<token_type>& token_types);
    std::unique_ptr<expression> expression_precedence();
    std::unique_ptr<expression> equality_precedence();
    std::unique_ptr<expression> comparison_precedence();
    std::unique_ptr<expression> term_precedence();
    std::unique_ptr<expression> factor_precedence();
    std::unique_ptr<expression> unary_precedence();
    std::unique_ptr<expression> primary_precedence();

    parser_exception error(const token& t, const std::string& msg);
    void synchronize();
};

NAMESPACE_END

#endif
