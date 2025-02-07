#ifndef JUMI_LUMINA_LEXER_H
#define JUMI_LUMINA_LEXER_H
#include "typedefs.h"
#include "tokens.h"
#include <optional>
#include <string>
#include <vector>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

class lexer
{
    struct lexer_state
    {
        uint32 current_line = 1;
        uint32 current_pos = 1;
        uint32 left_ptr = 0;
        uint32 right_ptr = 0;
        std::string input = "";
    };

public:
    lexer(const std::string& input);

    const std::vector<token>& get_tokens() const noexcept;

private:
    lexer_state _lexer_state;
    std::unordered_map<char, token(lexer::*)(void)> _character_map;
    std::unordered_map<std::string, token_type> _reserved_keyword_map;
    std::vector<token> _tokens;

    void tokenize();

    token fetch_token();
    std::optional<char> advance_lexer();
    std::optional<char> peek_next();
    bool advance_if_next_matches(char c);
    token create_token(token_type type, const literal_value& literal = std::monostate{});

    uint32 extract_lexeme_length() const noexcept;
    token left_paren();
    token right_paren();
    token left_brace();
    token right_brace();
    token comma();
    token dot();
    token minus();
    token plus();
    token semicolon();
    token colon();
    token question();
    token slash();
    token star();
    token bang();
    token equal();
    token greater();
    token less();
    token identifier();
    token string();
    token number();
    token invalid_token();
};

NAMESPACE_END

#endif
