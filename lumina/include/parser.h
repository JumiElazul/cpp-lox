#ifndef JUMI_LUMINA_PARSER_H
#define JUMI_LUMINA_PARSER_H
#include "typedefs.h"
#include "tokens.h"
#include <memory>
#include <vector>

NAMESPACE_BEGIN(lumina)

class expression;
class i_parser
{
public:
    virtual ~i_parser() = default;
    virtual void parse() = 0;

protected:
};

class recursive_descent_parser : public i_parser
{
public:
    recursive_descent_parser(const std::vector<token>& lexer_tokens);
    ~recursive_descent_parser() = default;

    virtual void parse() override;

    bool match(const std::vector<token_type>& token_types);
    std::unique_ptr<expression> expression_precedence();
    std::unique_ptr<expression> equality_precedence();
    std::unique_ptr<expression> comparison_precedence();
    std::unique_ptr<expression> term_precedence();
    std::unique_ptr<expression> factor_precedence();
    std::unique_ptr<expression> unary_precedence();
    std::unique_ptr<expression> primary_precedence();

private:
    const std::vector<token>& _lexer_tokens;
    uint32 _position;
};

NAMESPACE_END

#endif
