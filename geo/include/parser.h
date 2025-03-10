#ifndef JUMI_GEO_PARSER_H
#define JUMI_GEO_PARSER_H
#include "typedefs.h"
#include "tokens.h"
#include "exceptions.h"
#include "statements.h"
#include <initializer_list>
#include <memory>
#include <optional>
#include <vector>

NAMESPACE_BEGIN(geo)

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
    std::unique_ptr<statement> declaration_precedence();
    std::unique_ptr<statement> create_function_declaration_statement(std::string& kind);
    std::unique_ptr<statement> create_variable_declaration_statement();
    std::unique_ptr<statement> create_class_declaration_statement();
    std::unique_ptr<statement> statement_precedence();
    std::unique_ptr<statement> create_if_statement();
    std::unique_ptr<statement> create_while_statement();
    std::unique_ptr<statement> create_for_statement();
    std::unique_ptr<statement> create_break_statement();
    std::unique_ptr<statement> create_continue_statement();
    std::unique_ptr<statement> create_return_statement();
    std::vector<std::unique_ptr<statement>> create_block_statement();
    std::unique_ptr<statement> create_expression_statement();

    std::unique_ptr<expression> expression_precedence();
    std::unique_ptr<expression> assignment_precedence();
    std::unique_ptr<expression> logic_or_precedence();
    std::unique_ptr<expression> logic_and_precedence();
    std::unique_ptr<expression> equality_precedence();
    std::unique_ptr<expression> comparison_precedence();
    std::unique_ptr<expression> addition_precedence();
    std::unique_ptr<expression> multiplication_precedence();
    std::unique_ptr<expression> unary_precedence();
    std::unique_ptr<expression> postfix_precedence();
    std::unique_ptr<expression> call_precedence();
    std::unique_ptr<expression> primary_precedence();

    std::unique_ptr<expression> finish_call(std::unique_ptr<expression> expr);

    std::optional<token> advance_parser();
    std::optional<token> previous_token() const;
    std::optional<token> peek_next_token() const;
    token consume_if_matches(token_type type, const std::string& msg);
    bool check_type(token_type type);
    bool matches_token(std::initializer_list<token_type> token_types);
    void validate_binary_has_lhs(std::initializer_list<token_type> types);
    geo_runtime_error error(const std::string& msg, const token& t);
    void synchronize();
};

NAMESPACE_END

#endif
