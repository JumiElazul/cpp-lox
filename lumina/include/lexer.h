#ifndef JUMI_LUMINA_LEXER_H
#define JUMI_LUMINA_LEXER_H
#include "typedefs.h"
#include "tokens.h"
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

class lexer
{
struct lexer_state
{
    uint32 current_line;
    uint32 left_index;
    uint32 right_index;
    size_t input_length;
    std::string_view input;
};

public:
    lexer();
    std::vector<token> tokenize(const std::string& input);

private:
    lexer_state _lexer_state;
    std::unordered_map<char, void(lexer::*)(void)> _character_map;

    token fetch_token();
    char advance_lexer();
    char peek_next();
    token create_token(token_type type, const std::pair<uint32, uint32>& position, const std::string& lexeme, const literal_value& literal);
    void reset_lexer_state() noexcept;

    void left_paren();
    void right_paren();
};

NAMESPACE_END

#endif
