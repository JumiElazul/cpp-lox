#include "lexer.h"
#include "console_io.h"
#include "typedefs.h"
#include "tokens.h"
#include <optional>
#include <vector>
#include <string>
#include <unordered_map>

NAMESPACE_BEGIN(lumina)

lexer::lexer(console_io* io)
    : _lexer_state()
    , _character_map()
    , _io(io)
{
    _character_map =
    {
        { '(',  &lexer::left_paren     },
        { ')',  &lexer::right_paren    },
        { '{',  &lexer::left_brace     },
        { '}',  &lexer::right_brace    },
        { ',',  &lexer::comma          },
        { '.',  &lexer::dot            },
        { '-',  &lexer::minus          },
        { '+',  &lexer::plus           },
        { ';',  &lexer::semicolon      },
        { '/',  &lexer::slash          },
        { '*',  &lexer::star           },
        { '!',  &lexer::bang           },
        { '=',  &lexer::equal          },
        { '>',  &lexer::greater        },
        { '<',  &lexer::less           },
        { '\n', &lexer::newline        },
        { ' ',  &lexer::whitespace     },
        { '\r', &lexer::whitespace     },
        { '\t', &lexer::whitespace     },
        { '"',  &lexer::string         },
        { '1',  &lexer::number         },
        { '2',  &lexer::number         },
        { '3',  &lexer::number         },
        { '4',  &lexer::number         },
        { '5',  &lexer::number         },
        { '6',  &lexer::number         },
        { '7',  &lexer::number         },
        { '8',  &lexer::number         },
        { '9',  &lexer::number         },
        { '0',  &lexer::number         },
    };
}

std::vector<token> lexer::tokenize(const std::string& input)
{
    _lexer_state = lexer_state{};
    _lexer_state.input = std::move(input);

    std::vector<token> tokens;
    tokens.reserve(input.size() / 3);
    tokens.push_back(create_token(token_type::bof_, 0));

    while (_lexer_state.right_ptr < _lexer_state.input.size())
    {
        token next_token = fetch_token();

        switch (next_token.type)
        {
            case token_type::newline_:
            {
                _lexer_state.current_line += 1;
                _lexer_state.current_pos = 0;
            } break;
            case token_type::invalid_:
            {
                ++_lexer_state.current_pos;
            } break;
            default:
            {
                size_t lexeme_len = next_token.lexeme.size();
                _lexer_state.current_pos += static_cast<uint32>(lexeme_len);
                tokens.push_back(next_token);
            } break;
        }

    }

    return tokens;
}

token lexer::fetch_token()
{
    _lexer_state.left_ptr = _lexer_state.right_ptr;
    std::optional<char> current_char = advance_lexer();
    if (current_char.has_value())
    {
        auto char_map_find = _character_map.find(*current_char);
        if (char_map_find != _character_map.end())
        {
            return (this->*char_map_find->second)();
        }
    }
    return create_token(token_type::invalid_, 0);
}

std::optional<char> lexer::advance_lexer()
{
    if (_lexer_state.right_ptr >= _lexer_state.input.size())
        return std::nullopt;

    return _lexer_state.input[_lexer_state.right_ptr++];
}

std::optional<char> lexer::peek_next()
{
    if (_lexer_state.right_ptr >= _lexer_state.input.size())
        return std::nullopt;

    return _lexer_state.input[_lexer_state.right_ptr];
}

bool lexer::advance_if_next_matches(char c)
{
    if (peek_next() == c)
    {
        advance_lexer();
        return true;
    }

    return false;
}

token lexer::create_token(token_type type, uint32 length, const literal_value& literal)
{
    std::string lexeme = _lexer_state.input.substr(_lexer_state.left_ptr, length);
    return token{ type, lexeme, literal, { _lexer_state.current_line, _lexer_state.current_pos } };
}

uint32 lexer::extract_lexeme_length() const noexcept
{
    return _lexer_state.right_ptr - _lexer_state.left_ptr;
}

token lexer::left_paren()
{
    return create_token(token_type::left_paren_, 1);
}

token lexer::right_paren()
{
    return create_token(token_type::right_paren_, 1);
}

token lexer::left_brace()
{
    return create_token(token_type::left_brace_, 1);
}

token lexer::right_brace()
{
    return create_token(token_type::right_brace_, 1);
}

token lexer::comma()
{
    return create_token(token_type::comma_, 1);
}

token lexer::dot()
{
    return create_token(token_type::dot_, 1);
}

token lexer::minus()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::minus_equal_, 2);

    return create_token(token_type::minus_, 1);
}

token lexer::plus()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::plus_equal_, 2);

    return create_token(token_type::plus_, 1);
}

token lexer::semicolon()
{
    return create_token(token_type::semicolon_, 1);
}

token lexer::slash()
{
    if (advance_if_next_matches('='))
    {
        return create_token(token_type::slash_equal_, 2);
    }
    else if (advance_if_next_matches('/'))
    {
        while (peek_next().has_value() && peek_next() != '\n')
        {
            advance_lexer();
        }

        uint32 len = extract_lexeme_length();
        return create_token(token_type::invalid_, len);
    }

    return create_token(token_type::slash_, 1);
}

token lexer::star()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::star_equal_, 2);

    return create_token(token_type::star_, 1);
}

token lexer::bang()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::bang_equal_, 2);

    return create_token(token_type::bang_, 1);
}

token lexer::equal()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::equal_equal_, 2);

    return create_token(token_type::equal_, 1);
}

token lexer::greater()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::greater_equal_, 2);

    return create_token(token_type::greater_, 1);
}

token lexer::less()
{
    if (advance_if_next_matches('='))
        return create_token(token_type::less_equal_, 2);

    return create_token(token_type::less_, 1);
}

token lexer::newline()
{
    return create_token(token_type::newline_, 1);
}

token lexer::whitespace()
{
    return invalid_token();
}

token lexer::string()
{
    while (peek_next() != '"' && peek_next().has_value())
    {
        advance_lexer();
    }

    if (peek_next() == '"')
    {
        advance_lexer();
        uint32 literal_len = extract_lexeme_length() - 2;
        uint32 lexeme_len = extract_lexeme_length();
        std::string str = _lexer_state.input.substr(_lexer_state.left_ptr + 1, literal_len);

        return create_token(token_type::string_, lexeme_len, str);
    }

    // Untermintated string
    return invalid_token();
}

token lexer::number()
{
    while (peek_next().has_value() && isdigit(*peek_next()))
    {
        advance_lexer();
    }

    if (peek_next() == '.')
    {
        advance_lexer();
        while (peek_next().has_value() && isdigit(*peek_next()))
        {
            advance_lexer();
        }
    }

    uint32 len = extract_lexeme_length();
    double d = std::stod(_lexer_state.input.substr(_lexer_state.left_ptr, len));
    return create_token(token_type::number_, len, d);
}

token lexer::invalid_token()
{
    uint32 len = extract_lexeme_length();
    return create_token(token_type::invalid_, len);
}

NAMESPACE_END

