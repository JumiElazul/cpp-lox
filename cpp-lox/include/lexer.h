#ifndef JUMI_CPPLOX_LEXER_H
#define JUMI_CPPLOX_LEXER_H
#include "typedefs.h"
#include "tokens.h"
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

NAMESPACE_BEGIN(cpplox)

class console_io;

class lexer
{
    struct lexer_state
    {
        uint32 current_line = 1;
        uint32 current_pos = 1;
        uint32 left_ptr = 0;
        uint32 right_ptr = 0;
        std::string input = "";
        std::string_view current_source_line;
    };

public:
    lexer(const std::string& input, console_io* io = nullptr);

    const std::vector<token>& get_tokens() const noexcept;
    bool error_occurred() const noexcept;
    void reset_error_flag() noexcept;

private:
    lexer_state _lexer_state;
    std::vector<token> _tokens;
    bool _lexer_error;
    console_io* _io;

    static const std::unordered_map<char, token(lexer::*)(void)> char_to_lexer_func_map;
    static const std::unordered_map<std::string, token_type> reserved_keyword_lookup;

    void tokenize();

    token fetch_token();
    std::optional<char> advance_lexer();
    std::optional<char> peek_next();
    bool advance_if_next_matches(char c);
    token create_token(token_type type, const literal_value& literal = std::monostate{});

    std::string format_error(const token& t) const;
    std::string_view extract_source_line(std::string_view, size_t index);
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
    token modulo();
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
