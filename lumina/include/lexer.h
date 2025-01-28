#ifndef JUMI_LUMINA_LEXER_H
#define JUMI_LUMINA_LEXER_H
#include "typedefs.h"
#include "tokens.h"
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

class console_io;
class lexer
{
    struct lexer_state
    {
        uint32 current_line;
        uint32 current_pos;
        uint32 left_ptr;
        uint32 right_ptr;
        std::string input;
    };

public:
    lexer(console_io* io);
    std::vector<token> tokenize(const std::string& input);

private:
    lexer_state _lexer_state;
    std::unordered_map<char, token(lexer::*)(void)> _character_map;
    console_io* _io;

    token fetch_token();
    std::optional<char> advance_lexer();
    std::optional<char> peek_next();
    bool advance_if_next_matches(char c);
    token create_token(token_type type, uint32 start, uint32 length, const literal_value& literal = std::monostate{});

    token left_paren();
    token right_paren();
    token left_brace();
    token right_brace();
    token comma();
    token dot();
    token minus();
    token plus();
    token semicolon();
    token slash();
    token star();
    token bang();
    token equal();
    token greater();
    token less();
    token newline();
    token whitespace();
    token string();
    token number();
    token invalid_token();
};

NAMESPACE_END

#endif
